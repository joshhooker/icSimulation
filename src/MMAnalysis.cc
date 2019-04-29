#include "MMAnalysis.hh"

MMAnalysis* MMAnalysis::fInstance = NULL;

MMAnalysis::MMAnalysis() {}

MMAnalysis* MMAnalysis::Instance() {
  if(!fInstance) {
    fInstance =  new MMAnalysis();
  }
    return fInstance;
}

void MMAnalysis::SetFilename(const char* filename) {
  fFilename = filename;
}

void MMAnalysis::OpenFile() {
  fFile = new TFile(fFilename.c_str(),"recreate");

  fTree = new TTree("simData", "simulation data");

  fTree->Branch("gridEnergy", &fGridEnergy);
  fTree->Branch("gridID", &fGridID);
  fTree->Branch("gridTrackID", &fGridTrackID);

  fTree->Branch("scintXPosition", &fScintXPosition);
  fTree->Branch("scintYPosition", &fScintYPosition);
  fTree->Branch("scintZPosition", &fScintZPosition);
  fTree->Branch("scintEnergy", &fScintEnergy);
  fTree->Branch("scintTrackID", &fScintTrackID);
  fTree->Branch("scintMass", &fScintMass);
  fTree->Branch("scintCharge", &fScintCharge);

  fTree->Branch("beamEnergy", &fBeamEnergy);
  fTree->Branch("beamCharge", &fBeamCharge);
  fTree->Branch("beamMass", &fBeamMass);

  fAllTree = new TTree("allData", "all events");
  fAllTree->Branch("energy", &fEnergy);
  fAllTree->Branch("cmEnergy", &fCMEnergy);
  fAllTree->Branch("lighAngleCM", &fCMLightAngle);
  fAllTree->Branch("lightAngleLab", &fLabLightAngle);
  fAllTree->Branch("lightEnergy", &fLightEnergy);
  fAllTree->Branch("heavyAngleCM", &fCMHeavyAngle);
  fAllTree->Branch("heavyAngleLab", &fLabHeavyAngle);
  fAllTree->Branch("heavyEnergy", &fHeavyEnergy);
}

void MMAnalysis::CloseFile() {
  fTree->Write();
  fAllTree->Write();
  fFile->Close();
}

void MMAnalysis::SetGridEnergy(std::vector<G4double> energy) {
  fGridEnergy = energy;
}

void MMAnalysis::SetGridID(std::vector<G4int> id) {
  fGridID = id;
}

void MMAnalysis::SetGridTrackID(std::vector<G4int> trackID) {
  fGridTrackID = trackID;
}

void MMAnalysis::SetScintXPosition(std::vector<G4double> position) {
  fScintXPosition = position;
}

void MMAnalysis::SetScintYPosition(std::vector<G4double> position) {
  fScintYPosition = position;
}

void MMAnalysis::SetScintZPosition(std::vector<G4double> position) {
  fScintZPosition = position;
}

void MMAnalysis::SetScintEnergy(std::vector<G4double> energy) {
  fScintEnergy = energy;
}

void MMAnalysis::SetScintTime(std::vector<G4double> time) {
  fScintTime = time;
}

void MMAnalysis::SetScintTrackID(std::vector<G4int> trackID) {
  fScintTrackID = trackID;
}

void MMAnalysis::SetScintCharge(std::vector<G4int> charge) {
  fScintCharge = charge;
}

void MMAnalysis::SetScintMass(std::vector<G4int> mass) {
  fScintMass = mass;
}

void MMAnalysis::SetEnergy(G4double energy) {
  fEnergy = energy;
}

void MMAnalysis::SetCMEnergy(G4double energy) {
  fCMEnergy = energy;
}

void MMAnalysis::SetBeamEnergy(G4double energy) {
  fBeamEnergy = energy;
}

void MMAnalysis::SetBeamCharge(G4int charge) {
  fBeamCharge = charge;
}

void MMAnalysis::SetBeamMass(G4int mass) {
  fBeamMass = mass;
}

void MMAnalysis::SetCMLightAngle(G4double angle) {
  fCMLightAngle = angle;
}

void MMAnalysis::SetLabLightAngle(G4double angle) {
  fLabLightAngle = angle;
}

void MMAnalysis::SetLightEnergy(G4double energy) {
  fLightEnergy = energy;
}

void MMAnalysis::SetCMHeavyAngle(G4double angle) {
  fCMHeavyAngle = angle;
}

void MMAnalysis::SetLabHeavyAngle(G4double angle) {
  fLabHeavyAngle = angle;
}

void MMAnalysis::SetHeavyEnergy(G4double energy) {
  fHeavyEnergy = energy;
}

void MMAnalysis::Fill() {
  fTree->Fill();
}

void MMAnalysis::FillAll() {
  fAllTree->Fill();
}
