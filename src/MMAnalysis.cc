#include "MMAnalysis.hh"

MMAnalysis* MMAnalysis::fInstance = NULL;

MMAnalysis::MMAnalysis() {
}

MMAnalysis* MMAnalysis::Instance() {
  if(!fInstance) {
    fInstance =  new MMAnalysis();
  }
    return fInstance;
}

void MMAnalysis::SetFilename(const char* filename) {
  fFilename=filename;
}

void MMAnalysis::OpenFile() {
  fFile = new TFile(fFilename.c_str(),"recreate");

  fTree = new TTree("simData", "simulation data");
  fTree->Branch("gunEnergy", &fGunEnergy, "gunEnergy/D");

  fTree->Branch("icGrid1E", &fICGrid1E, "icGrid1E/D");
  fTree->Branch("icGrid2E", &fICGrid2E, "icGrid2E/D");
  fTree->Branch("icGrid3E", &fICGrid3E, "icGrid3E/D");
  fTree->Branch("icGrid4E", &fICGrid4E, "icGrid4E/D");
  fTree->Branch("icGridTotalE", &fICGridTotalE, "icGridTotalE/D");

  fTree->Branch("scintE", &fScintE, "scintE/D");
}

void MMAnalysis::CloseFile() {
  fTree->Write();
  fFile->Close();
}

void MMAnalysis::SetGunEnergy(double gunEnergy) {
  fGunEnergy = gunEnergy;
}

void MMAnalysis::SetICGrid1E(double energy) {
  fICGrid1E = energy;
}

void MMAnalysis::SetICGrid2E(double energy) {
  fICGrid2E = energy;
}

void MMAnalysis::SetICGrid3E(double energy) {
  fICGrid3E = energy;
}

void MMAnalysis::SetICGrid4E(double energy) {
  fICGrid4E = energy;
}

void MMAnalysis::SetICGridTotalE(double energy) {
  fICGridTotalE = energy;
}

void MMAnalysis::SetScintE(double energy) {
  fScintE = energy;
}

void MMAnalysis::Fill() {
  fTree->Fill();
}
