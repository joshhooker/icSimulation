#include "MMPrimaryGeneratorAction.hh"

MMPrimaryGeneratorAction::MMPrimaryGeneratorAction() :
  G4VUserPrimaryGeneratorAction() , fParticle(0) {

  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  gsl_rng_env_setup();
  T = gsl_rng_default;
  r = gsl_rng_alloc(T);
  gsl_rng_set(r, time(NULL));

  DefineCommands();
}

MMPrimaryGeneratorAction::~MMPrimaryGeneratorAction() {
  delete fParticleGun;
  delete fMessenger;
}

void MMPrimaryGeneratorAction::GeneratePrimaries(G4Event* g4Event) {

  G4IonTable* particleTable = G4IonTable::GetIonTable();
  G4String particleName;
  G4double kinE;

  double randNum = gsl_rng_uniform(r);

  if(randNum < 0.85) {
    // 7Be Beam
    fParticle = particleTable->GetIon(4, 7, 0.);
    kinE = 30.5;
  }
  else if(randNum >= 0.85 && randNum < 0.9) { // randNum >= 0.85 && randNum < 0.9
    // 7Li Background
    fParticle = particleTable->GetIon(3, 7, 0.);
    kinE = 20.02;
  }
  else if(randNum >= 0.9 && randNum < 0.95) { // randNum >= 0.9 && randNum < 0.95
    // 6Li Background
    fParticle = particleTable->GetIon(3, 6, 0.);
    kinE = 17.17;
  }
  else {
    // 4He Background
    fParticle = particleTable->GetIon(2, 4, 0.);
    kinE = 13.37;
  }

  G4double sigma = kinE*0.05;

  G4double beamXPos = gsl_ran_gaussian(r, 5.);
  G4double beamYPos = gsl_ran_gaussian(r, 5.);
  fParticleGun->SetParticlePosition(G4ThreeVector(beamXPos*mm, beamYPos*mm, -50.0*cm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));

  kinE = G4RandGauss::shoot(kinE*MeV,sigma*MeV);
  fParticleGun->SetParticleEnergy(kinE*MeV);

  fParticleGun->SetParticleDefinition(fParticle);

  fParticleGun->GeneratePrimaryVertex(g4Event);

  MMAnalysis* analysis = MMAnalysis::Instance();
  analysis->SetBeamEnergy(kinE*MeV);
  analysis->SetBeamCharge(fParticle->GetAtomicNumber());
  analysis->SetBeamMass(fParticle->GetAtomicMass());
}

void MMPrimaryGeneratorAction::DefineCommands() {
  fMessenger = new G4GenericMessenger(this,
				      "/MM/generator/",
				      "Primary generator control");

}
