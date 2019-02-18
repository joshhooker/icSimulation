#include "ProtonScatteringProcess.hh"

ProtonScatteringProcess::ProtonScatteringProcess(const G4String& processName)
  : G4VDiscreteProcess(processName,fHadronic), fScatteringEnergy(1.e6), fCMScatteringEnergy(0.) {
  SetProcessSubType(111);
}

ProtonScatteringProcess::~ProtonScatteringProcess() {
}

G4double ProtonScatteringProcess::GetMeanFreePath(const G4Track& aTrack,
					    G4double /*previousStepSize*/,
					    G4ForceCondition* condition) {

  G4ParticleDefinition* heavy = aTrack.GetDefinition();
  G4double heavyMass = heavy->GetAtomicMass();
  G4double energy = aTrack.GetKineticEnergy()/((heavyMass+1.)*MeV);

  G4double mfp = (energy>fScatteringEnergy||aTrack.GetTrackID()>1) ? DBL_MAX : 0.;

  *condition = NotForced;
  return mfp;
}

G4VParticleChange* ProtonScatteringProcess::PostStepDoIt( const G4Track& aTrack,
						    const G4Step& aStep) {

  G4StepPoint* preStepPoint = aStep.GetPreStepPoint();
  G4StepPoint* postStepPoint = aStep.GetPostStepPoint();

  const MMDetectorConstruction* detectorConstruction
      = static_cast<const MMDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  // fScintLogical = detectorConstruction->GetScint();
  // fHavarLogical = detectorConstruction->GetHavar();
  // fScoringVolume = detectorConstruction->GetScoringVolume();
  // fW1Logical = detectorConstruction->GetW1();
  // fW4Logical = detectorConstruction->GetW4();
  // fW7Logical = detectorConstruction->GetW7();

  if (postStepPoint->GetStepStatus()==fGeomBoundary) {
    return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);
  }

  // if (preStepPoint->GetTouchableHandle()
  //      ->GetVolume()->GetLogicalVolume()==fScintLogical ||
  //     postStepPoint->GetTouchableHandle()
  //      ->GetVolume()->GetLogicalVolume()==fScintLogical) {
  //   return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);
  // }

  // if (preStepPoint->GetTouchableHandle()
  //      ->GetVolume()->GetLogicalVolume()==fHavarLogical ||
  //     postStepPoint->GetTouchableHandle()
  //      ->GetVolume()->GetLogicalVolume()==fHavarLogical) {
  //   return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);
  // }

  // if (preStepPoint->GetTouchableHandle()
  //       ->GetVolume()->GetLogicalVolume()!=fScoringVolume &&
  //     postStepPoint->GetTouchableHandle()
  //       ->GetVolume()->GetLogicalVolume()!=fScoringVolume &&
  //     preStepPoint->GetTouchableHandle()
  //       ->GetVolume()->GetLogicalVolume()!=fW1Logical &&
  //     postStepPoint->GetTouchableHandle()
  //       ->GetVolume()->GetLogicalVolume()!=fW1Logical &&
  //     preStepPoint->GetTouchableHandle()
  //       ->GetVolume()->GetLogicalVolume()!=fW4Logical &&
  //     postStepPoint->GetTouchableHandle()
  //       ->GetVolume()->GetLogicalVolume()!=fW4Logical &&
  //     preStepPoint->GetTouchableHandle()
  //       ->GetVolume()->GetLogicalVolume()!=fW7Logical &&
  //     postStepPoint->GetTouchableHandle()
  //       ->GetVolume()->GetLogicalVolume()!=fW7Logical) {
  //   return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);
  // }

  aParticleChange.Initialize(aTrack);

  G4double energy = aTrack.GetKineticEnergy()/MeV;
  G4ParticleDefinition* heavy = aTrack.GetDefinition();
  G4double heavyMass = heavy->GetAtomicMass();

  G4double arg = (2.*G4UniformRand()-1.);
  G4double pAngleProtonCM = acos(arg);
  G4double aAngleProtonCM = 2.*3.14159*G4UniformRand();

  G4double protonEnergyLab = 4.*heavyMass/(heavyMass+1.)/(heavyMass+1.)*
    cos(pAngleProtonCM/2.)*cos(pAngleProtonCM/2.)*energy;
  G4double recoilEnergyLab = energy-protonEnergyLab;

  G4double pAngleRecoilLab = atan(sin(pAngleProtonCM)/(heavyMass-cos(pAngleProtonCM)));

  G4ThreeVector momentumDirection = aTrack.GetMomentumDirection();
  G4ThreeVector v = G4ThreeVector(0.,0.,1.).cross(momentumDirection);
  G4double rotAngle = acos(momentumDirection.z());

  G4ThreeVector protonLab(sin(pAngleProtonCM/2.)*cos(aAngleProtonCM),
			  sin(pAngleProtonCM/2.)*sin(aAngleProtonCM),
			  cos(pAngleProtonCM/2.));

  protonLab = protonLab.rotate(v,rotAngle);

  G4ThreeVector recoilLab(-1.*sin(pAngleRecoilLab)*cos(aAngleProtonCM),
			  -1.*sin(pAngleRecoilLab)*sin(aAngleProtonCM),
			  cos(pAngleRecoilLab));
  recoilLab = recoilLab.rotate(v,rotAngle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* proton = particleTable->FindParticle("proton");

  G4Track* sec1 = new G4Track(new G4DynamicParticle(proton, protonLab.unit(), protonEnergyLab*MeV),
			      aTrack.GetGlobalTime(),
			      aTrack.GetPosition());
  sec1->SetUserInformation(new MMTrackingInformation(energy/(heavyMass+1.),pAngleProtonCM,aAngleProtonCM,aTrack.GetPosition()));
  G4Track* sec2 = new G4Track(new G4DynamicParticle(heavy, recoilLab.unit(), recoilEnergyLab*MeV),
			      aTrack.GetGlobalTime(),
			      aTrack.GetPosition());

  aParticleChange.SetNumberOfSecondaries(2);
  aParticleChange.AddSecondary(sec1);
  aParticleChange.AddSecondary(sec2);

  aParticleChange.ProposeEnergy(0.);
  aParticleChange.ProposeTrackStatus(fStopAndKill);

  return &aParticleChange;
}

void ProtonScatteringProcess::StartTracking(G4Track* track) {
  G4VProcess::StartTracking(track);	// Apply base class actions

  G4ParticleDefinition* heavy = track->GetDefinition();
  G4double heavyMass = heavy->GetAtomicMass();
  //fScatteringEnergy = (fCMScatteringEnergy>0.) ? fCMScatteringEnergy*(heavyMass+1.) : track->GetKineticEnergy()*G4UniformRand()/MeV;
  //fScatteringEnergy = (fCMScatteringEnergy>0.) ? fCMScatteringEnergy : track->GetKineticEnergy()*G4UniformRand()/((heavyMass+1.)*MeV);
  if(fCMScatteringEnergy>0.) {
    fScatteringEnergy = fCMScatteringEnergy;
  }
  else {
    G4double randNum = G4UniformRand();

    // Uniform CM Energy Distribution
    fScatteringEnergy = track->GetKineticEnergy()*randNum/((heavyMass+1.)*MeV);
  }
}

void ProtonScatteringProcess::SetCMScatteringEnergy(G4double energy) {
  fCMScatteringEnergy = energy;
}
