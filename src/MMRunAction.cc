#include "MMRunAction.hh"

MMRunAction::MMRunAction(MMDetectorConstruction* detector, MMPrimaryGeneratorAction* primary) :
  G4UserRunAction(), fDetector(detector), fPrimary(primary) {
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // Create analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetFileName("sim");


}

MMRunAction::~MMRunAction() {
  delete G4AnalysisManager::Instance();
}

G4Run* MMRunAction::GenerateRun() {return (new MMRunData);}

void MMRunAction::BeginOfRunAction(const G4Run* run) {
  auto analysisManager = G4AnalysisManager::Instance();

//  if(run->GetRunID() == 0) {
    const MMEventAction* constEventAction = static_cast<const MMEventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
    MMEventAction* eventAction = const_cast<MMEventAction*>(constEventAction);

    analysisManager->CreateNtuple("simData", "simulation data");

    analysisManager->CreateNtupleDColumn("gridEnergy", eventAction->GetGridEnergy()); // columnID = 0
    analysisManager->CreateNtupleIColumn("gridID" , eventAction->GetGridID());        // columnID = 1

    analysisManager->CreateNtupleDColumn("scintXPosition", eventAction->GetScintXPosition()); // columnID = 2
    analysisManager->CreateNtupleDColumn("scintYPosition", eventAction->GetScintYPosition()); // columnID = 3
    analysisManager->CreateNtupleDColumn("scintEnergy", eventAction->GetScintEnergy());       // columnID = 4
    analysisManager->CreateNtupleIColumn("scintTrackID", eventAction->GetScintTrackID());     // columnID = 5
    analysisManager->CreateNtupleIColumn("scintMass", eventAction->GetScintMass());           // columnID = 6
    analysisManager->CreateNtupleIColumn("scintCharge", eventAction->GetScintCharge());       // columnID = 7

    analysisManager->CreateNtupleDColumn("beamEnergy"); // columnID = 8
    analysisManager->CreateNtupleIColumn("beamCharge"); // columnID = 9
    analysisManager->CreateNtupleIColumn("beamMass");   // columnID = 10

    analysisManager->CreateNtupleDColumn("energy");        // columnID = 11
    analysisManager->CreateNtupleDColumn("cmEnergy");      // columnID = 12
    analysisManager->CreateNtupleDColumn("vertex");        // columnID = 13
    analysisManager->CreateNtupleDColumn("qValue");        // columnID = 14
    analysisManager->CreateNtupleDColumn("excitedEnergy"); // columnID = 15

    analysisManager->CreateNtupleDColumn("lightAngleCM");  // columnID = 16
    analysisManager->CreateNtupleDColumn("lightAngleLab"); // columnID = 17
    analysisManager->CreateNtupleDColumn("lightEnergy");   // columnID = 18

    analysisManager->CreateNtupleDColumn("heavyAngleCM");  // columnID = 19
    analysisManager->CreateNtupleDColumn("heavyAngleLab"); // columnID = 20
    analysisManager->CreateNtupleDColumn("heavyEnergy");   // columnID = 21

    analysisManager->CreateNtupleIColumn("lightRecoilCharge"); // columnID = 22
    analysisManager->CreateNtupleIColumn("lightRecoilMass");   // columnID = 23
    analysisManager->CreateNtupleIColumn("heavyRecoilCharge"); // columnID = 24
    analysisManager->CreateNtupleIColumn("heavyRecoilMass");   // columnID = 25

    analysisManager->FinishNtuple();
//  }

  // Open an output file
  analysisManager->OpenFile();
}

void MMRunAction::EndOfRunAction(const G4Run*) {

  // save histograms & ntuple
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
