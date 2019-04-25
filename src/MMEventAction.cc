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
  std::vector<G4double> scintHitXPos;
  std::vector<G4double> scintHitYPos;
  std::vector<G4double> scintHitZPos;
  std::vector<G4double> scintHitE;
  std::vector<G4double> scintHitT;
  std::vector<G4int> scintHitTrackID;
  std::vector<G4int> scintHitCharge;
  std::vector<G4int> scintHitMass;
  G4double fScintE = 0.;
  for(G4int i = 0; i < (hScint->entries()); ++i) {
    G4ThreeVector pos = (*hScint)[i]->GetPosition();
    scintHitXPos.push_back(pos.x());
    scintHitYPos.push_back(pos.y());
    scintHitZPos.push_back(pos.z());
    G4double energy = (*hScint)[i]->GetEnergy();
    scintHitE.push_back(fRandom3->Gaus(energy, energy*fScintResolution));
    scintHitT.push_back((*hScint)[i]->GetTime());
    scintHitTrackID.push_back((*hScint)[i]->GetTrackID());
    scintHitCharge.push_back((*hScint)[i]->GetParticle()->GetAtomicNumber());
    scintHitMass.push_back((*hScint)[i]->GetParticle()->GetAtomicMass());
    fScintE += (*hScint)[i]->GetEnergy();
  }

  if(fWriteAllEvents) analysis->FillAll();

  if(fScintE/eV < 0.1) return;

  for(G4int i = 0; i < fNumGrids; i++) {
    G4double icGridEnergy = 0.;
    for(G4int j = 0; j < hICGridHC[i]->entries(); ++j) {
      icGridEnergy += (*hICGridHC[i])[j]->GetEnergy();
    }
    G4double icGridEnergyRes = 2.35*sqrt(fFanoFactor*fWorkFunction*(1e-6)*icGridEnergy);
    icGridEnergy = fRandom3->Gaus(icGridEnergy, icGridEnergyRes);
    icGridEnergy = fRandom3->Gaus(icGridEnergy, icGridEnergy*fGridResolution);
    fICGridE[i] = icGridEnergy;
  }

  G4double icGridETotal = 0.;
  for(G4int i = 0; i < fNumGrids; i++) {
    icGridETotal += fICGridE[i];
  }

  analysis->SetScintXPos(scintHitXPos);
  analysis->SetScintXPos(scintHitYPos);
  analysis->SetScintXPos(scintHitZPos);
  analysis->SetScintE(scintHitE);
  analysis->SetScintT(scintHitT);
  analysis->SetScintTrackID(scintHitTrackID);
  analysis->SetScintCharge(scintHitCharge);
  analysis->SetScintMass(scintHitMass);

  analysis->SetICGridEnergy(fICGridE);
  analysis->SetICGridTotalEnergy(icGridETotal);

  analysis->Fill();
}
