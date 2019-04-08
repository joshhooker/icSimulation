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

  G4String excitedname = aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName();

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

  if((excitedname != "Be7") && (excitedname != "Li6") && (excitedname != "Li7") &&
    (excitedname != "B8") && (excitedname != "He4")) {
    mfp = 0.;
  }

  MMAnalysis* analysis = MMAnalysis::Instance();
  analysis->SetReactionType(0);

  *condition = NotForced;
  return mfp;
}

G4VParticleChange* BinaryReactionProcess::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep) {
  G4StepPoint* preStepPoint = aStep.GetPreStepPoint();
  G4StepPoint* postStepPoint = aStep.GetPostStepPoint();

  if(postStepPoint->GetStepStatus() == fGeomBoundary) {
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  const MMDetectorConstruction* detectorConstruction = static_cast<const MMDetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  G4LogicalVolume* fWorldLogical = detectorConstruction->GetWorldVolume();

  if(preStepPoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume() != fWorldLogical &&
     postStepPoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume() != fWorldLogical) {
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  G4String beamName = aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName();
  // If it's 8B in an excited state, decay to proton and 7Be
  if(beamName == "B8[768.500]") return Decay(aTrack, 1, 1, 4, 7);

  aParticleChange.Initialize(aTrack);

  // Randomly choose if populating excited states
  G4int reactionType = 1;
  G4double excitedEnergy = 0.;
  G4double qValue = fQValue;
  if(G4UniformRand() > 0.5) {
    reactionType = 2;
    qValue -= fExcitedStateEnergy;
    excitedEnergy = fExcitedStateEnergy;
  }

  G4double energy = aTrack.GetKineticEnergy()/MeV;
  G4double totalEnergy = energy + qValue;
  G4ParticleDefinition* projectile = aTrack.GetDefinition();
  G4double projectileMass = projectile->GetAtomicMass();

  G4double B = static_cast<G4double>(projectileMass)*static_cast<G4double>(fLightProductMass)/(static_cast<G4double>(projectileMass) +
    static_cast<G4double>(fTargetMass))/(static_cast<G4double>(fLightProductMass) + static_cast<G4double>(fHeavyProductMass))*(energy/totalEnergy);
  G4double D = static_cast<G4double>(fTargetMass)*static_cast<G4double>(fHeavyProductMass)/(static_cast<G4double>(projectileMass) +
    static_cast<G4double>(fTargetMass))/(static_cast<G4double>(fLightProductMass) + static_cast<G4double>(fHeavyProductMass))*
    (1. + static_cast<G4double>(projectileMass)/static_cast<G4double>(fTargetMass)*qValue/totalEnergy);

  G4double maxAngle = (B < D) ? M_PI : asin(sqrt(D/B));

  G4double arg = (1. - cos(maxAngle))*G4UniformRand() + cos(maxAngle);
  // G4double pAngleLightLab = acos(arg);
  G4double pAngleLightLab = M_PI*G4UniformRand();
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

  G4double pAngleHeavyLab = asin(sqrt(static_cast<G4double>(fLightProductMass)/(static_cast<G4double>(fHeavyProductMass))*
    lightEnergyLab/heavyEnergyLab)*sin(pAngleLightLab));
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
  if(particleTable->GetIonTable()->FindIon(fHeavyProductCharge, fHeavyProductMass, excitedEnergy))
    heavy = particleTable->GetIonTable()->FindIon(fHeavyProductCharge, fHeavyProductMass, excitedEnergy);
  else heavy = particleTable->GetIonTable()->GetIon(fHeavyProductCharge, fHeavyProductMass, excitedEnergy);

  // G4cout << heavy->GetParticleName() << G4endl;

  G4Track* sec1 = new G4Track(new G4DynamicParticle(light,lightLab.unit(), lightEnergyLab*MeV),
                  aTrack.GetGlobalTime(), aTrack.GetPosition());
  sec1->SetUserInformation(new MMTrackingInformation(reactionType, energy, energy*fTargetMass/(static_cast<G4double>(projectileMass)
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

  // G4cout << "Reaction" << G4endl;
  // G4cout << reactionType << '\t' << heavy->GetParticleName() << '\t' << aTrack.GetTrackID() << G4endl;

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

G4VParticleChange* BinaryReactionProcess::Decay(const G4Track& aTrack, G4int lightCharge, G4int lightMass,
  G4int heavyCharge, G4int heavyMass) {
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  // Setup Particle 1
  G4DynamicParticle* particle1 = new G4DynamicParticle;
  G4ParticleDefinition* particle1Def;
  if(lightCharge == 0 && lightMass == 1) {
    G4String particleName;
    particle1Def = particleTable->FindParticle(particleName="neutron");
  }
  else {
    if(particleTable->GetIonTable()->FindIon(lightCharge, lightMass, 0.)) {
      particle1Def = particleTable->GetIonTable()->FindIon(lightCharge, lightMass, 0.);
    }
    else particle1Def = particleTable->GetIonTable()->GetIon(lightCharge, lightMass, 0.);
  }
  particle1->SetDefinition(particle1Def);
  G4double particle1Mass = particle1Def->GetPDGMass()/CLHEP::amu_c2;

  // Setup Particle 2
  G4DynamicParticle* particle2 = new G4DynamicParticle;
  G4ParticleDefinition* particle2Def = G4IonTable::GetIonTable()->GetIon(heavyCharge, heavyMass, 0.);
  particle2->SetDefinition(particle2Def);
  G4double particle2Mass = particle2Def->GetPDGMass()/CLHEP::amu_c2;

  G4double qValue = aTrack.GetDynamicParticle()->GetDefinition()->GetPDGMass() -
    (particle1Def->GetPDGMass() + particle2Def->GetPDGMass());
  G4double cmEnergy = qValue;

  if(cmEnergy < 0.) return &aParticleChange; // Below the threshold

  // Generate random CM Angles
  G4double cmTheta = M_PI*G4UniformRand();; // 0 to pi
  G4double cmPhi = 2.*M_PI*G4UniformRand(); // 0 to 2pi

  G4double p1 = sqrt(2.*particle1->GetMass()*cmEnergy*particle2Mass/(particle1Mass + particle2Mass));
  G4double p2 = sqrt(2.*particle2->GetMass()*cmEnergy*particle1Mass/(particle1Mass + particle2Mass));

  // Get momentum directions
  G4ThreeVector p_new_1 = G4ThreeVector(p1*sin(cmTheta)*sin(cmPhi), p1*sin(cmTheta)*cos(cmPhi), p1*cos(cmTheta));
  G4ThreeVector p_new_2 = -p_new_1;
  G4ThreeVector p_parent = aTrack.GetMomentum();
  p_new_1 += p_parent*(particle1Mass/(particle1Mass + particle2Mass));
  p_new_2 += p_parent*(particle2Mass/(particle1Mass + particle2Mass));
  particle1->SetMomentum(p_new_1);
  particle2->SetMomentum(p_new_2);

  G4double total_mom_1 = p_new_1.getR();
  G4double total_mom_2 = p_new_2.getR();
  particle1->SetKineticEnergy((total_mom_1*total_mom_1)/(2.*particle1->GetMass()));
  particle2->SetKineticEnergy((total_mom_2*total_mom_2)/(2.*particle2->GetMass()));

  G4Track* sec1 = new G4Track(particle1, aTrack.GetGlobalTime(), aTrack.GetPosition());
  G4Track* sec2 = new G4Track(particle2, aTrack.GetGlobalTime(), aTrack.GetPosition());

  aParticleChange.AddSecondary(sec1);
  aParticleChange.AddSecondary(sec2);
  aParticleChange.ProposeEnergy(0.);
  aParticleChange.ProposeTrackStatus(fStopAndKill);

  // G4cout << "Q Value: " << qValue << G4endl;
  // G4cout << "Particle 1 Mass: " << particle1Mass << '\t' << "Particle 2 Mass: " << particle2Mass << G4endl;
  // G4cout << "Before KE: " << aTrack.GetKineticEnergy() << G4endl;
  // G4cout << "Particle 1 KE: " << particle1->GetKineticEnergy() << G4endl;
  // G4cout << "Particle 2 KE: " << particle2->GetKineticEnergy() << G4endl;

  return &aParticleChange;
}

