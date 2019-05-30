#include "BinaryReactionProcess.hh"
#include "NucleonStates.hh"
#include "TypeDef.hh"

BinaryReactionProcess::BinaryReactionProcess(const G4String& processName)
  : G4VDiscreteProcess(processName, fHadronic), fScatteringEnergy(1e6) {
  SetProcessSubType(111);
}

BinaryReactionProcess::~BinaryReactionProcess() {}

G4double BinaryReactionProcess::GetMeanFreePath(const G4Track& aTrack, G4double /*previousStepSize*/,
                                                G4ForceCondition* condition) {
  G4double energy = aTrack.GetKineticEnergy()/MeV;

  G4int particleMass = aTrack.GetDefinition()->GetAtomicMass();
  G4int particleCharge = aTrack.GetDefinition()->GetAtomicNumber();

  const MMDetectorConstruction* detectorConstruction = static_cast<const MMDetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4LogicalVolume* fWorldLogical = detectorConstruction->GetWorldVolume();
  G4LogicalVolume* fTargetLogical = detectorConstruction->GetTargetVolume();
  G4LogicalVolume* currentVolume = aTrack.GetStep()->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  G4String excitedname = aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName();

  G4double position = aTrack.GetPosition().z();

  G4double mfp = (energy <= fScatteringEnergy &&
                  aTrack.GetTrackID() == 1 &&
                  (currentVolume == fTargetLogical ||
                  (currentVolume == fWorldLogical && position > -0.0066 && position < 0.1))) ? 0. : DBL_MAX;

  // if(mfp < 1) G4cout << fScatteringEnergy << '\t' << energy << '\t' << aTrack.GetTrackID() << '\t' << currentVolume->GetName() << '\t' << mfp << '\t' << aTrack.GetPosition().z() << G4endl;
  // G4cout << fScatteringEnergy << '\t' << energy << '\t' << aTrack.GetTrackID() << '\t' << currentVolume->GetName() << '\t' << mfp << '\t' << aTrack.GetPosition().z() << G4endl;

  // Look at excited name and see if it's in an excited state
  size_t pos = excitedname.find('[');
  double measuredExcitedEnergy = 0.;
  G4String beamNameEnergy = "";
  if(pos > 1000) {
    measuredExcitedEnergy = 0.;
  }
  else {
    beamNameEnergy = excitedname.substr(pos + 1, std::string::npos);
    beamNameEnergy.pop_back();
    measuredExcitedEnergy = std::atof(beamNameEnergy.c_str())/1000.;
  }

  // Check if above threshold and needs to decay
  NucleonStates* states = NucleonStates::Instance();
  auto thresholds = states->GetThresholds(particleCharge, particleMass);

  if(measuredExcitedEnergy > 0 && !thresholds.empty()) {
    if(measuredExcitedEnergy > thresholds[0].thresholdEnergy) {
      mfp = 0.;
    }
  }

  if(excitedname == "Be8") {
    mfp = 0.;
  }

  *condition = NotForced;
  return mfp;
}

G4VParticleChange* BinaryReactionProcess::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep) {

  // Determine if particle needs to decay due to excited state
  G4String incomingParticleName = aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName();
  size_t pos = incomingParticleName.find('[');
  double measuredExcitedEnergy = 0.;
  G4String incomingParticleNameEnergy = "";
  if(pos > 100) {
    measuredExcitedEnergy = 0.;
  }
  else {
    incomingParticleNameEnergy = incomingParticleName.substr(pos + 1, std::string::npos);
    incomingParticleNameEnergy.pop_back();
    measuredExcitedEnergy = std::atof(incomingParticleNameEnergy.c_str())/1000.;
  }

  NucleonStates* states = NucleonStates::Instance();

  // Get thresholds
  auto thresholds = states->GetThresholds(aTrack.GetParticleDefinition()->GetAtomicNumber(),
    aTrack.GetParticleDefinition()->GetAtomicMass());

  if(!thresholds.empty()) {
    thresholdStruct threshold = thresholds[0];
    if(measuredExcitedEnergy > threshold.thresholdEnergy) {
      return Decay(aTrack, threshold.lightDecayCharge, threshold.lightDecayMass, threshold.heavyDecayCharge, threshold.heavyDecayMass);
    }
  }

  G4StepPoint* preStepPoint = aStep.GetPreStepPoint();
  G4StepPoint* postStepPoint = aStep.GetPostStepPoint();

  if(postStepPoint->GetStepStatus() == fGeomBoundary) {
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  const MMDetectorConstruction* detectorConstruction = static_cast<const MMDetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  G4LogicalVolume* fWorldLogical = detectorConstruction->GetWorldVolume();

  aParticleChange.Initialize(aTrack);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  G4ParticleDefinition* neutron = particleTable->FindParticle(particleName="neutron");
  G4ParticleDefinition* deutron = particleTable->GetIonTable()->GetIon(1, 2, 0.0);
  G4ParticleDefinition* carbon = particleTable->GetIonTable()->GetIon(6, 12, 0.0);

  // Get beam properties
  G4int beamCharge = aTrack.GetParticleDefinition()->GetAtomicNumber();
  G4int beamMass = aTrack.GetParticleDefinition()->GetAtomicMass();
  G4double beamMassPDG = aTrack.GetParticleDefinition()->GetPDGMass();

  G4int targetCharge, targetMass;
  G4int lightRecoilCharge, lightRecoilMass;
  G4int heavyRecoilCharge, heavyRecoilMass;
  G4double targetMassPDG, lightRecoilMassPDG, heavyRecoilMassPDG;

  // Determine if (d, d), (d, n) or (c, c)
  // 90% of the time choose the deutron as the target
  G4ParticleDefinition* targetDef = G4UniformRand() > 0.9 ? carbon : deutron;

  targetCharge = targetDef->GetAtomicNumber();
  targetMass = targetDef->GetAtomicMass();
  targetMassPDG = targetDef->GetPDGMass();

  G4double energy = aTrack.GetKineticEnergy()/MeV;
  G4double cmEnergy = energy*targetMassPDG/(beamMassPDG + targetMassPDG);

  G4ParticleDefinition* lightRecoilDef;
  // 80% of the time choose neutron for (d, n) instead of (d, d)
  if(targetDef == deutron) {
    lightRecoilDef = G4UniformRand() > 0.8 ? deutron : neutron;
  }
  // Only (c, c)
  else {
    lightRecoilDef = carbon;
  }

  lightRecoilCharge = lightRecoilDef->GetAtomicNumber();
  lightRecoilMass = lightRecoilDef->GetAtomicMass();
  lightRecoilMassPDG = lightRecoilDef->GetPDGMass();

  // Figure out heavy particle charge and mass
  heavyRecoilCharge = beamCharge + targetCharge - lightRecoilCharge;
  heavyRecoilMass = beamMass + targetMass - lightRecoilMass;

  // Sample on excited states
  G4double exProbability = G4UniformRand();
  G4double excitedEnergy = states->GetExcitedLevel(heavyRecoilCharge, heavyRecoilMass, exProbability);

  // Get Heavy Ion Particle Definition
  G4ParticleDefinition* heavyRecoilDef = particleTable->GetIonTable()->GetIon(heavyRecoilCharge, heavyRecoilMass, excitedEnergy);
  heavyRecoilMassPDG = heavyRecoilDef->GetPDGMass();

  // If the light recoil is heavier than the heavy recoil, switch them
  if(lightRecoilMass > heavyRecoilMass) {
    std::swap(lightRecoilCharge, heavyRecoilCharge);
    std::swap(lightRecoilMass, heavyRecoilMass);
    std::swap(lightRecoilMassPDG, heavyRecoilMassPDG);
    std::swap(lightRecoilDef, heavyRecoilDef);
  }

  G4double qValue = beamMassPDG + targetMassPDG - (lightRecoilMassPDG + heavyRecoilMassPDG);

  // Check if reaction is possible
  if(cmEnergy + qValue < 0) {
    G4cout << "Not enough energy: " << incomingParticleName << '\t' << "; Energy: " << energy << "; CM Energy: " << cmEnergy << G4endl;
    G4cout << "\t Q Value: " << qValue << "; Vertex Location: " << aTrack.GetPosition().z() << G4endl;
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  G4double totalEnergy = energy + qValue;

  G4double B = beamMassPDG*lightRecoilMassPDG/(beamMassPDG + targetMassPDG)/(lightRecoilMassPDG + heavyRecoilMassPDG)*
              (energy/totalEnergy);
  G4double D = targetMassPDG*heavyRecoilMassPDG/(beamMassPDG + targetMassPDG)/(lightRecoilMassPDG + heavyRecoilMassPDG)*
              (1. + beamMassPDG/targetMassPDG*qValue/totalEnergy);

  G4double maxAngle = (B < D) ? M_PI : asin(sqrt(D/B));
  // G4double arg = (1. - cos(maxAngle))*G4UniformRand() + cos(maxAngle);
  // G4double pAngleLightLab = acos(arg);
  G4double pAngleLightLab = maxAngle*G4UniformRand();
  G4double aAngleLightLab = 2.*M_PI*G4UniformRand();

  G4ThreeVector lightVector(sin(pAngleLightLab)*cos(aAngleLightLab), sin(pAngleLightLab)*sin(aAngleLightLab),
                            cos(pAngleLightLab));
  G4ThreeVector beamVector(0., 0., 1.);

  G4double lightEnergyLab = (B <= D) ? totalEnergy*B*pow(cos(pAngleLightLab) + sqrt(D/B - sin(pAngleLightLab)*sin(pAngleLightLab)), 2.) :
                            totalEnergy*B*pow(cos(pAngleLightLab) - sqrt(D/B - sin(pAngleLightLab)*sin(pAngleLightLab)), 2.);
  G4double lightEnergyLab2 = (B <= D) ? totalEnergy*B*pow(cos(pAngleLightLab + 0.001) + sqrt(D/B - sin(pAngleLightLab + 0.001)*sin(pAngleLightLab + 0.001)), 2.) :
                            totalEnergy*B*pow(cos(pAngleLightLab + 0.001) - sqrt(D/B - sin(pAngleLightLab + 0.001)*sin(pAngleLightLab + 0.001)), 2.);

  G4double heavyEnergyLab = totalEnergy - lightEnergyLab;

  // Make sure the energies of both recoils are greater than 0
  if(lightEnergyLab < 0 || heavyEnergyLab < 0) {
    G4cout << lightEnergyLab << '\t' << heavyEnergyLab << G4endl;
  }

  G4double val = sqrt(lightEnergyLab/totalEnergy/D)*sin(pAngleLightLab);
  G4double val2 = sqrt(lightEnergyLab2/totalEnergy/D)*sin(pAngleLightLab + 0.001);
  G4double pAngleLightCM = (val2 > val) ? asin(val) : M_PI - asin(val);
  G4double aAngleLightCM = aAngleLightLab;

  G4double pAngleHeavyLab = asin(sqrt(lightRecoilMassPDG/(heavyRecoilMassPDG)*
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

  G4Track* sec1 = new G4Track(new G4DynamicParticle(lightRecoilDef,lightLab.unit(), lightEnergyLab*MeV),
                  aTrack.GetGlobalTime(), aTrack.GetPosition());
  sec1->SetUserInformation(new MMTrackingInformation(energy, cmEnergy, pAngleLightCM,
                                                     pAngleLightLab, aAngleLightCM, pAngleHeavyCM, pAngleHeavyLab,
                                                     lightEnergyLab, heavyEnergyLab, aTrack.GetPosition(), qValue, excitedEnergy,
                                                     lightRecoilDef, heavyRecoilDef));
  G4Track* sec2 = new G4Track(new G4DynamicParticle(heavyRecoilDef, heavyLab.unit(), heavyEnergyLab*MeV),
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
  G4double beamEnergyMin = beamEnergy/2.;

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

  return &aParticleChange;
}

