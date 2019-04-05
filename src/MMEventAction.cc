#include "MMEventAction.hh"

MMEventAction::MMEventAction(G4int numGrids) :
  G4UserEventAction(),
  fScintE(0.)
{
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
  fScintE = 0.;
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
  fScintE = 0.;
  for(G4int i = 0; i < (hScint->entries()); ++i) {
    fScintE += (*hScint)[i]->GetTotalEnergy();
    fScintMass = (*hScint)[0]->GetMass();
    fScintCharge = (*hScint)[0]->GetCharge();
    fScintXPos = (*hScint)[0]->GetXPosition()/mm;
    fScintYPos = (*hScint)[0]->GetYPosition()/mm;
    fScintZPos = (*hScint)[0]->GetZPosition()/mm;
  }

  if(fWriteAllEvents) analysis->FillAll();

  if(fScintE < 0.001) return;

  fScintE = fRandom3->Gaus(fScintE, fScintE*fScintResolution);

  for(G4int i = 0; i < fNumGrids; i++) {
    G4double icGridEnergy = 0.;
    for(G4int j = 0; j < hICGridHC[i]->entries(); ++j) {
      icGridEnergy += (*hICGridHC[i])[j]->GetTotalEnergy();
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

  analysis->SetScintE(fScintE);
  analysis->SetScintMass(fScintMass);
  analysis->SetScintCharge(fScintCharge);
  analysis->SetScintXPos(fScintXPos);
  analysis->SetScintYPos(fScintYPos);
  analysis->SetScintZPos(fScintZPos);

  analysis->SetICGridEnergy(fICGridE);
  analysis->SetICGridTotalEnergy(icGridETotal);

  analysis->Fill();
}
