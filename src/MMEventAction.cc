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
  G4long seed = floor(2147483647*G4UniformRand());
  gsl_rng_set(r,seed);
}

MMEventAction::~MMEventAction() {
  gsl_rng_free (r);
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
  double scintEnergy = 0.;
  for(G4int i = 0; i < (hScint->entries()); ++i) {
    scintEnergy += (*hScint)[i]->GetTotalEnergy();
  }

  if(scintEnergy < 0.001) return;

  for(G4int i = 0; i < fNumGrids; i++) {
    G4double icGridEnergy = 0.;
    for(G4int j = 0; j < hICGridHC[i]->entries(); ++j) {
      icGridEnergy += (*hICGridHC[i])[j]->GetTotalEnergy();
    }
    G4double icGridEnergyRes = 2.35*sqrt(fFanoFactor*fWorkFunction*(1e-6)*icGridEnergy);
    icGridEnergy = G4RandGauss::shoot(icGridEnergy, icGridEnergyRes);
    fICGridE[i] = icGridEnergy;
  }

  G4double icGridETotal = 0.;
  for(G4int i = 0; i < fNumGrids; i++) {
    icGridETotal += fICGridE[i];
  }

  fScintE = scintEnergy;

  analysis->SetScintE(fScintE);

  analysis->SetICGridEnergy(fICGridE);
  analysis->SetICGridTotalEnergy(icGridETotal);

  analysis->Fill();
}

void MMEventAction::ParseParams(const std::map<std::string, G4double> &params) {
  for(std::map<std::string, G4double>::const_iterator it = params.begin(); it != params.end(); it++) {
    if(it->first == "fanoFactor") {
      fFanoFactor = it->second;
      G4cout << "SET: Fano Factor: " << fFanoFactor << G4endl;
    }
    else if(it->first == "workFunction") {
      fWorkFunction = it->second;
      G4cout << "SET: Work Function (eV): " << fWorkFunction << G4endl;
    }
  }
}
