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
  fAllTree->Branch("lightAngleLab", &fLightAngleLab);

  fAllTree->Branch("lightEnergyLab", &fLightEnergyLab);
  fAllTree->Branch("heavyAngleLab", &fHeavyAngleLab);
  fAllTree->Branch("heavyEnergyLab", &fHeavyEnergyLab);
}

void MMAnalysis::CloseFile() {
  fTree->Write();
  fAllTree->Write();
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

void MMAnalysis::SetScintMass(int mass) {
  fScintMass = mass;
}

void MMAnalysis::SetScintCharge(int charge) {
  fScintCharge = charge;
}

void MMAnalysis::SetScintXPos(double position) {
  fScintXPos = position;
}

void MMAnalysis::SetScintYPos(double position) {
  fScintYPos = position;
}

void MMAnalysis::SetScintZPos(double position) {
  fScintZPos = position;
}

void MMAnalysis::SetLightAngleLab(double angle) {
  fLightAngleLab = angle;
}

void MMAnalysis::SetLightEnergyLab(double energy) {
  fLightEnergyLab = energy;
}

void MMAnalysis::SetHeavyAngleLab(double angle) {
  fHeavyAngleLab = angle;
}

void MMAnalysis::SetHeavyEnergyLab(double energy) {
  fHeavyEnergyLab = energy;
}

void MMAnalysis::Fill() {
  fTree->Fill();
}

void MMAnalysis::FillAll() {
  fAllTree->Fill();
}
