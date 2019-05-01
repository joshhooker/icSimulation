#include "MMEventAction.hh"

MMEventAction::MMEventAction(G4int numGrids) :
  G4UserEventAction() {
  fNumGrids = numGrids;

  for(G4int i = 0; i < fNumGrids; i++) {
    fICGridE.push_back(0.);
    fICGridHCID.push_back(-1);
  }

  fScintHCID = -1;

  r = gsl_rng_alloc(gsl_rng_taus);
  G4long seed = static_cast<G4long>(floor(2147483647*G4UniformRand()));
  gsl_rng_set(r, seed);
  fRandom3 = new TRandom3(seed);
}

MMEventAction::~MMEventAction() {
  gsl_rng_free(r);
  delete fRandom3;
}

void MMEventAction::BeginOfEventAction(const G4Event*) {
  for(G4int i = 0; i < fNumGrids; i++) {
    fICGridE[i] = 0.;
  }

  char name[256];
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  for(G4int i = 0; i < fNumGrids; i++) {
    sprintf(name,"grid%d/genCollection", i + 1);
    fICGridHCID[i] = sdManager->GetCollectionID(name);
  }
  sprintf(name,"scint/genCollection");
  fScintHCID = sdManager->GetCollectionID(name);
}

void MMEventAction::EndOfEventAction(const G4Event* event) {

  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if(!hce) {
    G4ExceptionDescription msg;
    msg << "No hits collection of this event found.\n";
    G4Exception("MMEventAction::EndOfEventAction()",
		"MMCode001", JustWarning, msg);
    return;
  }

  MMGenHitsCollection* hICGridHC[fNumGrids];
  for(int i = 0; i < fNumGrids; i++) {
    hICGridHC[i] = static_cast<MMGenHitsCollection*>(hce->GetHC(fICGridHCID[i]));
  }

  MMGenHitsCollection* hScint;
  hScint = static_cast<MMGenHitsCollection*>(hce->GetHC(fScintHCID));

  MMAnalysis* analysis = MMAnalysis::Instance();

  // scintillator
  std::vector<G4double> scintHitXPosition;
  std::vector<G4double> scintHitYPosition;
  std::vector<G4double> scintHitZPosition;
  std::vector<G4double> scintHitEnergy;
  std::vector<G4double> scintHitTime;
  std::vector<G4int> scintHitTrackID;
  std::vector<G4int> scintHitCharge;
  std::vector<G4int> scintHitMass;
  G4double fScintE = 0.;
  for(G4int i = 0; i < (hScint->entries()); ++i) {
    G4ThreeVector pos = (*hScint)[i]->GetPosition();
    scintHitXPosition.push_back(pos.x());
    scintHitYPosition.push_back(pos.y());
    scintHitZPosition.push_back(pos.z());
    G4double energy = (*hScint)[i]->GetEnergy();
    scintHitEnergy.push_back(fRandom3->Gaus(energy, energy*fScintResolution));
    scintHitTime.push_back((*hScint)[i]->GetTime());
    scintHitTrackID.push_back((*hScint)[i]->GetTrackID());
    scintHitCharge.push_back((*hScint)[i]->GetParticle()->GetAtomicNumber());
    scintHitMass.push_back((*hScint)[i]->GetParticle()->GetAtomicMass());
    fScintE += (*hScint)[i]->GetEnergy();
  }

  // grids
  std::vector<G4double> gridHitEnergy;
  std::vector<G4int> gridHitID;
  std::vector<G4int> gridHitTrackID;
  for(G4int i = 0; i < fNumGrids; i++) {
    for(G4int j = 0; j < hICGridHC[i]->entries(); ++j) {
      G4double energy = (*hICGridHC[i])[j]->GetEnergy();
      G4double energyRes = 2.35*sqrt(fFanoFactor*fWorkFunction*(1e-6)*energy);
      energy = fRandom3->Gaus(energy, energyRes);
      energy = fRandom3->Gaus(energy, energy*fGridResolution);
      gridHitEnergy.push_back(energy);
      gridHitID.push_back(i);
      gridHitTrackID.push_back((*hICGridHC[i])[j]->GetTrackID());
    }
  }

  analysis->SetGridEnergy(gridHitEnergy);
  analysis->SetGridID(gridHitID);
  analysis->SetGridTrackID(gridHitTrackID);

  analysis->SetScintXPosition(scintHitXPosition);
  analysis->SetScintYPosition(scintHitYPosition);
  analysis->SetScintZPosition(scintHitZPosition);
  analysis->SetScintEnergy(scintHitEnergy);
  analysis->SetScintTime(scintHitTime);
  analysis->SetScintTrackID(scintHitTrackID);
  analysis->SetScintCharge(scintHitCharge);
  analysis->SetScintMass(scintHitMass);

  analysis->Fill();

  analysis->SetEnergy(0.);
  analysis->SetCMEnergy(0.);
  analysis->SetCMLightAngle(0.);
  analysis->SetLabLightAngle(0.);
  analysis->SetLightEnergy(0.);
  analysis->SetCMHeavyAngle(0.);
  analysis->SetLabHeavyAngle(0.);
  analysis->SetHeavyEnergy(0.);
  analysis->SetQValue(0.);
  analysis->SetExcitedEnergy(0.);
  analysis->SetLightRecoilCharge(0);
  analysis->SetLightRecoilMass(0);
  analysis->SetHeavyRecoilCharge(0);
  analysis->SetHeavyRecoilMass(0);
}
