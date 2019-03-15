#include "BinaryReactionProcess.hh"

BinaryReactionProcess::BinaryReactionProcess(const G4String& processName)
  : G4VDiscreteProcess(processName, fHadronic), fScatteringEnergy(1e6) {
  SetProcessSubType(111);
  fQValue = 0.;
}

BinaryReactionProcess::~BinaryReactionProcess() {}

G4double BinaryReactionProcess::GetMeanFreePath(const G4Track& aTrack, G4double /*previousStepSize*/,
                                                G4ForceCondition* condition) {
  G4double energy = aTrack.GetKineticEnergy()/MeV;

  G4int particleMass = aTrack.GetDefinition()->GetAtomicMass();
  G4int particleCharge = aTrack.GetDefinition()->GetAtomicNumber();

  const MMDetectorConstruction* detectorConstruction = static_cast<const MMDetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4LogicalVolume* fDetectLogical = detectorConstruction->GetDetectVolume();
  G4LogicalVolume* fFoilLogical = detectorConstruction->GetFoilVolume();
  G4LogicalVolume* fScintLogical = detectorConstruction->GetScintVolume();
  G4LogicalVolume* currentVolume = aTrack.GetStep()->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  G4double mfp = (energy > fScatteringEnergy ||
                  aTrack.GetTrackID() > 1 ||
                  currentVolume == fDetectLogical ||
                  currentVolume == fFoilLogical ||
                  currentVolume == fScintLogical ||
                  particleMass != 7 ||
                  particleCharge != 4) ? DBL_MAX : 0.;

  for(G4int i = 0; i < fNumGrids; i++) {
    if(currentVolume == detectorConstruction->GetGridVolume(i)) {
      mfp = DBL_MAX;
    }
  }

  *condition = NotForced;
  return mfp;
}

G4VParticleChange* BinaryReactionProcess::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep) {
  G4StepPoint* preStepPoint = aStep.GetPreStepPoint();
  G4StepPoint* postStepPoint = aStep.GetPostStepPoint();

  if(postStepPoint->GetStepStatus() == fGeomBoundary) {
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  if(aTrack.GetDefinition()->GetAtomicMass() != 7) {
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  const MMDetectorConstruction* detectorConstruction = static_cast<const MMDetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  G4LogicalVolume* fWorldLogical = detectorConstruction->GetWorldVolume();

  if(preStepPoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume() != fWorldLogical &&
     postStepPoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume() != fWorldLogical) {
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  aParticleChange.Initialize(aTrack);

  G4double energy = aTrack.GetKineticEnergy()/MeV;
  G4double totalEnergy = energy + fQValue;
  G4ParticleDefinition* projectile = aTrack.GetDefinition();
  G4double projectileMass = projectile->GetAtomicMass();

  G4double B = projectileMass*fLightProductMass/(projectileMass + fTargetMass)/(fLightProductMass +
               fHeavyProductMass)*(energy/totalEnergy);
  G4double D = fTargetMass*fHeavyProductMass/(projectileMass + fTargetMass)/(fLightProductMass +
               fHeavyProductMass)*(1. + projectileMass/fTargetMass*fQValue/totalEnergy);

  G4double maxAngle = (B < D) ? M_PI : asin(sqrt(D/B));

  G4double arg = (1. - cos(maxAngle))*G4UniformRand() + cos(maxAngle);
  G4double pAngleLightLab = acos(arg);
  G4double aAngleLightLab = 2.*M_PI*G4UniformRand();
  G4ThreeVector lightVector(sin(pAngleLightLab)*cos(aAngleLightLab), sin(pAngleLightLab)*sin(aAngleLightLab),
                            cos(pAngleLightLab));
  G4ThreeVector beamVector(0., 0., 1.);

  G4double lightEnergyLab = (B <= D) ? totalEnergy*B*pow(cos(pAngleLightLab) +
                                       sqrt(D/B - sin(pAngleLightLab)*sin(pAngleLightLab)), 2.) :
    totalEnergy*B*pow(cos(pAngleLightLab) - sqrt(D/B - sin(pAngleLightLab)*sin(pAngleLightLab)), 2.);
  G4double lightEnergyLab2 = (B <= D) ? totalEnergy*B*pow(cos(pAngleLightLab + 0.001) +
                                                          sqrt(D/B - sin(pAngleLightLab + 0.001)*sin(pAngleLightLab + 0.001)), 2.) :
    totalEnergy*B*pow(cos(pAngleLightLab + 0.001) - sqrt(D/B - sin(pAngleLightLab + 0.001)*sin(pAngleLightLab + 0.001)), 2.);

  G4double heavyEnergyLab = totalEnergy - lightEnergyLab;

  G4double val = sqrt(lightEnergyLab/totalEnergy/D)*sin(pAngleLightLab);
  G4double val2 = sqrt(lightEnergyLab2/totalEnergy/D)*sin(pAngleLightLab + 0.001);
  G4double pAngleLightCM = (val2 > val) ? asin(val) : M_PI - asin(val);
  G4double aAngleLightCM = aAngleLightLab;

  G4double pAngleHeavyLab = asin(sqrt(static_cast<G4double>(fLightProductMass)/
                                      (static_cast<G4double>(fHeavyProductMass))*lightEnergyLab/heavyEnergyLab)*sin(pAngleLightLab));
  G4ThreeVector heavyVector(-1.*sin(pAngleHeavyLab)*cos(aAngleLightLab), -1.*sin(pAngleHeavyLab)*sin(aAngleLightLab),
                            cos(pAngleHeavyLab));
  G4double pAngleHeavyCM = M_PI - pAngleLightCM;

  G4ThreeVector momentumDirection = aTrack.GetMomentumDirection();
  G4ThreeVector v = beamVector.cross(momentumDirection);
  G4double rotAngle = acos(momentumDirection.z());

  G4ThreeVector lightLab(sin(pAngleLightLab)*cos(aAngleLightLab),
			 sin(pAngleLightLab)*sin(aAngleLightLab),
			 cos(pAngleLightLab));
  lightLab = lightLab.rotate(v, rotAngle);

  G4ThreeVector heavyLab(-1.*sin(pAngleHeavyLab)*cos(aAngleLightLab),
       -1.*sin(pAngleHeavyLab)*sin(aAngleLightLab),
			 cos(pAngleHeavyLab));
  heavyLab = heavyLab.rotate(v, rotAngle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* light;
  if(fLightProductCharge == 0 && fLightProductMass == 1) { // Neutron
    G4String particleName;
    light = particleTable->FindParticle(particleName="neutron");
  }
  else {
    if(particleTable->GetIonTable()->FindIon(fLightProductCharge, fLightProductMass, 0.0)) {
      light = particleTable->GetIonTable()->FindIon(fLightProductCharge, fLightProductMass, 0.0);
    }
    else light = particleTable->GetIonTable()->GetIon(fLightProductCharge, fLightProductMass, 0.0);
  }

  G4ParticleDefinition* heavy;
  if(particleTable->GetIonTable()->FindIon(fHeavyProductCharge, fHeavyProductMass, 0.0))
    heavy = particleTable->GetIonTable()->FindIon(fHeavyProductCharge, fHeavyProductMass, 0.0);
  else heavy = particleTable->GetIonTable()->GetIon(fHeavyProductCharge, fHeavyProductMass, 0.0);

  G4Track* sec1 = new G4Track(new G4DynamicParticle(light,lightLab.unit(), lightEnergyLab*MeV),
                  aTrack.GetGlobalTime(), aTrack.GetPosition());
  sec1->SetUserInformation(new MMTrackingInformation(energy, energy*fTargetMass/(static_cast<G4double>(projectileMass)
                                                     + static_cast<G4double>(fTargetMass)), pAngleLightCM,
                                                     pAngleLightLab, aAngleLightCM, pAngleHeavyCM, pAngleHeavyLab,
                                                     lightEnergyLab, heavyEnergyLab, aTrack.GetPosition()));
  G4Track* sec2 = new G4Track(new G4DynamicParticle(heavy, heavyLab.unit(), heavyEnergyLab*MeV),
                  aTrack.GetGlobalTime(), aTrack.GetPosition());

  aParticleChange.SetNumberOfSecondaries(2);
  aParticleChange.AddSecondary(sec1);
  aParticleChange.AddSecondary(sec2);

  aParticleChange.ProposeEnergy(0.);
  aParticleChange.ProposeTrackStatus(fStopAndKill);

  return &aParticleChange;
}

void BinaryReactionProcess::StartTracking(G4Track* track) {
  G4VProcess::StartTracking(track);	// Apply base class actions

  // To make interaction happen anywhere from beam energy to 0
  // fScatteringEnergy = track->GetKineticEnergy()*G4UniformRand()/MeV;

  // To make interaction happen anywhere from beam energy to beam energy - 20 MeV
  G4double beamEnergy = track->GetKineticEnergy()/MeV;
  G4double beamEnergyMin = 20.*MeV;

  fScatteringEnergy = (beamEnergy - beamEnergyMin)*G4UniformRand() + beamEnergyMin;
}

