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
  fFilename = filename;
}

void MMAnalysis::OpenFile() {
  fFile = new TFile(fFilename.c_str(),"recreate");

  fTree = new TTree("simData", "simulation data");
  fTree->Branch("gunEnergy", &fGunEnergy, "gunEnergy/D");

  fTree->Branch("icGridEnergy", &fICGridEnergy);
  fTree->Branch("icGridTotalEnergy", &fICGridTotalEnergy, "icGridTotalE/D");

  fTree->Branch("scintE", &fScintE, "scintE/D");
}

void MMAnalysis::CloseFile() {
  fTree->Write();
  fFile->Close();
}

void MMAnalysis::SetGunEnergy(double gunEnergy) {
  fGunEnergy = gunEnergy;
}

void MMAnalysis::SetICGridEnergy(std::vector<G4double>& v) {
  fICGridEnergy = v;
}

void MMAnalysis::SetICGridTotalEnergy(double energy) {
  fICGridTotalEnergy = energy;
}

void MMAnalysis::SetScintE(double energy) {
  fScintE = energy;
}

void MMAnalysis::Fill() {
  fTree->Fill();
}
