#include "BinaryReactionProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "MMTrackingInformation.hh"
#include "G4IonTable.hh"

BinaryReactionProcess::BinaryReactionProcess(const G4String& processName)
  : G4VDiscreteProcess(processName,fHadronic), fScatteringEnergy(1e6) {
  SetProcessSubType(111);
  fQValue = 0.;
}

BinaryReactionProcess::~BinaryReactionProcess() {
}

G4double BinaryReactionProcess::GetMeanFreePath(const G4Track& aTrack,
					    G4double /*previousStepSize*/,
					    G4ForceCondition* condition) {

  G4double energy = aTrack.GetKineticEnergy()/MeV;
  
  G4double mfp = (energy>fScatteringEnergy||aTrack.GetTrackID()>1) ? DBL_MAX : 0.;

  *condition = NotForced;
  return mfp;
}

G4VParticleChange* BinaryReactionProcess::PostStepDoIt( const G4Track& aTrack,
						    const G4Step& aStep) {
  
  G4StepPoint* postStepPoint = aStep.GetPostStepPoint();
  if (postStepPoint->GetStepStatus()==fGeomBoundary) {
    return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);
  }

  aParticleChange.Initialize(aTrack);

  G4double energy = aTrack.GetKineticEnergy()/MeV;
  G4double totalEnergy = energy+fQValue;
  G4ParticleDefinition* projectile = aTrack.GetDefinition();
  G4double projectileMass = projectile->GetAtomicMass();
  
  G4double B = projectileMass*fLightProductMass/(projectileMass+fTargetMass)/(fLightProductMass+fHeavyProductMass)*(energy/totalEnergy);
  G4double D = fTargetMass*fHeavyProductMass/(projectileMass+fTargetMass)/(fLightProductMass+fHeavyProductMass)*(1.+projectileMass/fTargetMass*fQValue/totalEnergy);

  G4double maxAngle = (B<D) ? 3.14159 : asin(sqrt(D/B));

  G4double arg = (1.-cos(maxAngle))*G4UniformRand()+cos(maxAngle);
  G4double pAngleLightLab = acos(arg);
  G4double aAngleLightLab = 2.*3.14159*G4UniformRand();

  printf("B: %f D: %f\n",B,D);

  G4double lightEnergyLab = (B<=D) ? totalEnergy*B*pow(cos(pAngleLightLab)+sqrt(D/B-sin(pAngleLightLab)*sin(pAngleLightLab)),2.) :
    totalEnergy*B*pow(cos(pAngleLightLab)-sqrt(D/B-sin(pAngleLightLab)*sin(pAngleLightLab)),2.);
  G4double lightEnergyLab2 = (B<=D) ? totalEnergy*B*pow(cos(pAngleLightLab+0.001)+sqrt(D/B-sin(pAngleLightLab+0.001)*sin(pAngleLightLab+0.001)),2.) :
    totalEnergy*B*pow(cos(pAngleLightLab+0.001)-sqrt(D/B-sin(pAngleLightLab+0.001)*sin(pAngleLightLab+0.001)),2.);
  G4double heavyEnergyLab = totalEnergy-lightEnergyLab;

  G4double val = sqrt(lightEnergyLab/totalEnergy/D)*sin(pAngleLightLab);
  G4double val2 = sqrt(lightEnergyLab2/totalEnergy/D)*sin(pAngleLightLab+0.001);
  G4double pAngleLightCM = (val2>val) ? asin(val) : 3.14159-asin(val); 
  G4double aAngleLightCM = aAngleLightLab; 

  G4double pAngleHeavyLab = asin(sqrt(fLightProductMass/(fHeavyProductMass)*lightEnergyLab/heavyEnergyLab)*sin(pAngleLightLab));

  G4ThreeVector momentumDirection = aTrack.GetMomentumDirection();
  G4ThreeVector v = G4ThreeVector(0.,0.,1.).cross(momentumDirection);
  G4double rotAngle = acos(momentumDirection.z());

  G4ThreeVector lightLab(sin(pAngleLightLab)*cos(aAngleLightLab),
			 sin(pAngleLightLab)*sin(aAngleLightLab),
			 cos(pAngleLightLab));
  lightLab = lightLab.rotate(v,rotAngle);

  G4ThreeVector heavyLab(-1.*sin(pAngleHeavyLab)*cos(aAngleLightLab),
			 -1.*sin(pAngleHeavyLab)*sin(aAngleLightLab),
			 cos(pAngleHeavyLab));
  heavyLab = heavyLab.rotate(v,rotAngle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* light;
  if(particleTable->GetIonTable()->FindIon(fLightProductCharge,fLightProductMass,0.0)) 
    light = particleTable->GetIonTable()->FindIon(fLightProductCharge,fLightProductMass,0.0);
  else light = particleTable->GetIonTable()->GetIon(fLightProductCharge,fLightProductMass,0.0);

  G4ParticleDefinition* heavy;
  if(particleTable->GetIonTable()->FindIon(fHeavyProductCharge,fHeavyProductMass,0.0)) 
    heavy = particleTable->GetIonTable()->FindIon(fHeavyProductCharge,fHeavyProductMass,0.0);
  else heavy = particleTable->GetIonTable()->GetIon(fHeavyProductCharge,fHeavyProductMass,0.0);

  printf("IncomingZ: %d incomingM: %d recoilZ: %d recoilM: %d \n Lab Energy Incoming: %f Light Product Energy: %f Heavy Product Energy: %f \n Light Product Angle: %f Heavy Product Angle %f\n",
	 projectile->GetAtomicNumber(),projectile->GetAtomicMass(),heavy->GetAtomicNumber(),heavy->GetAtomicMass(),
	 energy,lightEnergyLab,heavyEnergyLab,pAngleLightLab,pAngleHeavyLab);

  G4Track* sec1 = new G4Track(new G4DynamicParticle(light,lightLab.unit(), lightEnergyLab*MeV),
			      aTrack.GetGlobalTime(),
			      aTrack.GetPosition());
  sec1->SetUserInformation(new MMTrackingInformation(energy*fTargetMass/(projectileMass+fTargetMass),pAngleLightCM,pAngleLightLab,
						     aAngleLightCM,aTrack.GetPosition()));
  G4Track* sec2 = new G4Track(new G4DynamicParticle(heavy, heavyLab.unit(), heavyEnergyLab*MeV),
			      aTrack.GetGlobalTime(),
			      aTrack.GetPosition());

  aParticleChange.SetNumberOfSecondaries(2);
  aParticleChange.AddSecondary(sec1);
  aParticleChange.AddSecondary(sec2);

  aParticleChange.ProposeEnergy(0.);
  aParticleChange.ProposeTrackStatus(fStopAndKill);

  return &aParticleChange;
}

void BinaryReactionProcess::StartTracking(G4Track* track) {
  G4VProcess::StartTracking(track);	// Apply base class actions

  fScatteringEnergy = track->GetKineticEnergy()*G4UniformRand()/MeV;
}

void BinaryReactionProcess::ParseParams(std::map<std::string,double> &params) {
    double lightProductMass = -1;
    double lightProductCharge = -1;
    double heavyProductMass = -1;
    double heavyProductCharge = -1;
    double targetMass = -1;
    double targetCharge = -1;
    for(std::map<std::string,double>::const_iterator it = params.begin();
	it!= params.end();it++) {
      if(it->first == "qValue"  ) {
	SetQValue(it->second);
      } else if(it->first == "lightProductMass" ) {
	lightProductMass = it->second;
      } else if(it->first == "lightProductCharge" ) {
	lightProductCharge = it->second;
      } else if(it->first == "heavyProductMass" ) {
	heavyProductMass = it->second;
      } else if(it->first == "heavyProductCharge" ) {
	heavyProductCharge = it->second;
      } else if(it->first == "targetMass" ) {
	targetMass = it->second;
      } else if(it->first == "targetCharge" ) {
	targetCharge = it->second;
      } 
    }
    if(lightProductCharge>0 && lightProductMass>0) {
      SetLightProduct(lightProductCharge,lightProductMass);
    }
    if(heavyProductCharge>0 && heavyProductMass>0) {
      SetHeavyProduct(heavyProductCharge,heavyProductMass);
    }
    if(targetCharge>0 && targetMass>0) {
      SetTarget(targetCharge,targetMass);
    }
}


