#include "MMAnalysis.hh"

MMAnalysis* MMAnalysis::fInstance = NULL;

MMAnalysis::MMAnalysis() : fGridEnergy(0), fGridID(0), fGridTrackID(0), fScintXPosition(0), fScintYPosition(0),
  fScintZPosition(0), fScintEnergy(0), fScintTime(0), fScintTrackID(0), fScintCharge(0), fScintMass(0), fEnergy(0),
  fCMEnergy(0), fVertex(-1000.), fBeamEnergy(0), fBeamCharge(0), fBeamMass(0), fCMLightAngle(0), fLabLightAngle(0),
  fLightEnergy(0), fCMHeavyAngle(0), fLabHeavyAngle(0), fHeavyEnergy(0), fQValue(0), fExcitedEnergy(0),
  fLightRecoilCharge(0), fLightRecoilMass(0), fHeavyRecoilCharge(0), fHeavyRecoilMass(0) {}

MMAnalysis* MMAnalysis::Instance() {
  if(!fInstance) {
    fInstance =  new MMAnalysis();
  }
    return fInstance;
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

  fTree->Branch("energy", &fEnergy);
  fTree->Branch("cmEnergy", &fCMEnergy);
  fTree->Branch("vertex", &fVertex);

  fTree->Branch("lightAngleCM", &fCMLightAngle);
  fTree->Branch("lightAngleLab", &fLabLightAngle);
  fTree->Branch("lightEnergy", &fLightEnergy);
  fTree->Branch("heavyAngleCM", &fCMHeavyAngle);
  fTree->Branch("heavyAngleLab", &fLabHeavyAngle);
  fTree->Branch("heavyEnergy", &fHeavyEnergy);
  fTree->Branch("qValue", &fQValue);
  fTree->Branch("excitedEnergy", &fExcitedEnergy);
  fTree->Branch("lightRecoilCharge", &fLightRecoilCharge);
  fTree->Branch("lightRecoilMass", &fLightRecoilMass);
  fTree->Branch("heavyRecoilCharge", &fHeavyRecoilCharge);
  fTree->Branch("heavyRecoilMass", &fHeavyRecoilMass);
}