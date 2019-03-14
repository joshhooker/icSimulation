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
  fTree->Branch("gunEnergy", &fGunEnergy, "gunEnergy/D");

  fTree->Branch("icGridEnergy", &fICGridEnergy);
  fTree->Branch("icGridTotalEnergy", &fICGridTotalEnergy, "icGridTotalE/D");

  fTree->Branch("scintE", &fScintE, "scintE/D");
  fTree->Branch("scintMass", &fScintMass);
  fTree->Branch("scintCharge", &fScintCharge);
  fTree->Branch("scintXPos", &fScintXPos);
  fTree->Branch("scintYPos", &fScintYPos);
  fTree->Branch("scintZPos", &fScintZPos);

  fAllTree = new TTree("allData", "all events");
  fAllTree->Branch("lightAngleLab", &fLabLightAngle);
  fAllTree->Branch("lightEnergy", &fLightEnergy);
  fAllTree->Branch("heavyAngleLab", &fLabHeavyAngle);
  fAllTree->Branch("heavyEnergy", &fHeavyEnergy);
}

void MMAnalysis::CloseFile() {
  fTree->Write();
  fAllTree->Write();
  fFile->Close();
}

void MMAnalysis::SetGunEnergy(G4double gunEnergy) {
  fGunEnergy = gunEnergy;
}

void MMAnalysis::SetICGridEnergy(std::vector<G4double>& v) {
  fICGridEnergy = v;
}

void MMAnalysis::SetICGridTotalEnergy(G4double energy) {
  fICGridTotalEnergy = energy;
}

void MMAnalysis::SetScintE(G4double energy) {
  fScintE = energy;
}

void MMAnalysis::SetScintMass(int mass) {
  fScintMass = mass;
}

void MMAnalysis::SetScintCharge(int charge) {
  fScintCharge = charge;
}

void MMAnalysis::SetScintXPos(G4double position) {
  fScintXPos = position;
}

void MMAnalysis::SetScintYPos(G4double position) {
  fScintYPos = position;
}

void MMAnalysis::SetScintZPos(G4double position) {
  fScintZPos = position;
}

void MMAnalysis::SetLabLightAngle(G4double angle) {
  fLabLightAngle = angle;
}

void MMAnalysis::SetLightEnergy(G4double energy) {
  fLightEnergy = energy;
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
