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

    fGridEnergy.clear();
    fGridID.clear();

    fScintXPosition.clear();
    fScintYPosition.clear();
    fScintEnergy.clear();
    fScintTrackID.clear();
    fScintMass.clear();
    fScintCharge.clear();

    fBeamEnergy = 0.;
    fBeamCharge = 0;
    fBeamMass = 0;

    fEnergy = 0.;
    fCMEnergy = 0.;
    fVertexZ = 0.;
    fQValue = 0.;
    fExcitedEnergy = 0.;

    fLightAngleCM = 0.;
    fLightAngleLab = 0.;
    fLightEnergy = 0.;
    fHeavyAngleCM = 0.;
    fHeavyAngleLab = 0.;
    fHeavyEnergy = 0.;

    fLightRecoilCharge = 0;
    fLightRecoilMass = 0;
    fHeavyRecoilCharge = 0;
    fHeavyRecoilMass = 0;

//    auto runData = MMRunData::Instance();
//    runData->Reset();
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
        G4double gridEnergy = 0.;
        for(G4int j = 0; j < hICGridHC[i]->entries(); ++j) {
            gridEnergy += (*hICGridHC[i])[j]->GetEnergy();
        }
        G4double energyRes = 2.35*sqrt(fFanoFactor*fWorkFunction*(1e-6)*gridEnergy);
        gridEnergy = fRandom3->Gaus(gridEnergy, energyRes);
        gridEnergy = fRandom3->Gaus(gridEnergy, gridEnergy*fGridResolution);
        gridHitEnergy.push_back(gridEnergy);
        gridHitID.push_back(i);
    }

    fGridEnergy = gridHitEnergy;
    fGridID = gridHitID;

    fScintXPosition = scintHitXPosition;
    fScintYPosition = scintHitYPosition;
    fScintEnergy = scintHitEnergy;
    fScintTrackID = scintHitTrackID;
    fScintMass = scintHitMass;
    fScintCharge = scintHitCharge;

    G4PrimaryVertex* primaryVertex = event->GetPrimaryVertex();
    G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary();
    fBeamEnergy = primaryParticle->GetKineticEnergy();
    fBeamCharge = primaryParticle->GetParticleDefinition()->GetAtomicNumber();
    fBeamMass = primaryParticle->GetParticleDefinition()->GetAtomicMass();

    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->FillNtupleDColumn(8, fBeamEnergy);
    analysisManager->FillNtupleIColumn(9, fBeamCharge);
    analysisManager->FillNtupleIColumn(10, fBeamMass);

    analysisManager->FillNtupleDColumn(11, fEnergy);
    analysisManager->FillNtupleDColumn(12, fCMEnergy);
    analysisManager->FillNtupleDColumn(13, fVertexZ);
    analysisManager->FillNtupleDColumn(14, fQValue);
    analysisManager->FillNtupleDColumn(15, fExcitedEnergy);

    analysisManager->FillNtupleDColumn(16, fLightAngleCM);
    analysisManager->FillNtupleDColumn(17, fLightAngleLab);
    analysisManager->FillNtupleDColumn(18, fLightEnergy);

    analysisManager->FillNtupleDColumn(19, fHeavyAngleCM);
    analysisManager->FillNtupleDColumn(20, fHeavyAngleLab);
    analysisManager->FillNtupleDColumn(21, fHeavyEnergy);

    analysisManager->FillNtupleIColumn(22, fLightRecoilCharge);
    analysisManager->FillNtupleIColumn(23, fLightRecoilMass);
    analysisManager->FillNtupleIColumn(24, fHeavyRecoilCharge);
    analysisManager->FillNtupleIColumn(25, fHeavyRecoilMass);

    analysisManager->AddNtupleRow();

//    auto runData = MMRunData::Instance();
//    runData->FillPerEvent();
}
