#include "MMEventAction.hh"

MMEventAction::MMEventAction() :
  G4UserEventAction(),
  fScintE(0.),
  fICGrid1E(0.), fICGrid2E(0),
  fICGrid3E(0.), fICGrid4E(0)
{
  for(int i = 0; i < 4; i++) fICGridHCID[i] = -1;
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
  fICGrid1E = 0;
  fICGrid2E = 0;
  fICGrid3E = 0;
  fICGrid4E = 0;

  char name[256];
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  for(int i = 0; i < 4; i++) {
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

  MMGenHitsCollection* hICGridHC[4];
  for(int i = 0; i < 4; i++) {
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

  // Fix this later
  G4double icGrid1Energy = 0.;
  for(G4int i = 0; i < hICGridHC[0]->entries(); ++i) {
    icGrid1Energy += (*hICGridHC[0])[i]->GetTotalEnergy();
  }
  G4double icGrid1EnergyRes = 2.35*sqrt(fFanoFactor*fWorkFunction*(1e-6)*icGrid1Energy);
  icGrid1Energy = G4RandGauss::shoot(icGrid1Energy, icGrid1EnergyRes);

  G4double icGrid2Energy = 0.;
  for(G4int i = 0; i < hICGridHC[1]->entries(); ++i) {
    icGrid2Energy += (*hICGridHC[1])[i]->GetTotalEnergy();
  }
  G4double icGrid2EnergyRes = 2.35*sqrt(fFanoFactor*fWorkFunction*(1e-6)*icGrid2Energy);
  icGrid2Energy = G4RandGauss::shoot(icGrid2Energy, icGrid2EnergyRes);

  G4double icGrid3Energy = 0.;
  for(G4int i = 0; i < hICGridHC[2]->entries(); ++i) {
    icGrid3Energy += (*hICGridHC[2])[i]->GetTotalEnergy();
  }
  G4double icGrid3EnergyRes = 2.35*sqrt(fFanoFactor*fWorkFunction*(1e-6)*icGrid3Energy);
  icGrid3Energy = G4RandGauss::shoot(icGrid3Energy, icGrid3EnergyRes);

  G4double icGrid4Energy = 0.;
  for(G4int i = 0; i < hICGridHC[3]->entries(); ++i) {
    icGrid4Energy += (*hICGridHC[3])[i]->GetTotalEnergy();
  }
  G4double icGrid4EnergyRes = 2.35*sqrt(fFanoFactor*fWorkFunction*(1e-6)*icGrid4Energy);
  icGrid4Energy = G4RandGauss::shoot(icGrid4Energy, icGrid4EnergyRes);

  fScintE = scintEnergy;

  fICGrid1E = icGrid1Energy;
  fICGrid2E = icGrid2Energy;
  fICGrid3E = icGrid3Energy;
  fICGrid4E = icGrid4Energy;

  analysis->SetScintE(fScintE);

  analysis->SetICGrid1E(fICGrid1E);
  analysis->SetICGrid2E(fICGrid2E);
  analysis->SetICGrid3E(fICGrid3E);
  analysis->SetICGrid4E(fICGrid4E);
  analysis->SetICGridTotalE(fICGrid1E + fICGrid2E + fICGrid3E + fICGrid4E);

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
