#include "MMRunData.hh"

MMRunData* MMRunData::fInstance = NULL;

MMRunData::MMRunData() : G4Run() {
//  fEnergy = 0.;
//  fCMEnergy = 0.;
//  fVertexZ = 0.;
//
//  fBeamEnergy = 0.;
//  fBeamCharge = 0;
//  fBeamMass = 0;
}

MMRunData* MMRunData::Instance() {
  if(!fInstance) {
    fInstance = new MMRunData();
  }
  return fInstance;
}

MMRunData::~MMRunData() {;}

void MMRunData::FillPerEvent() {
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  if(fBeamEnergy < 1) {
      G4cout << "MMRunData: " << fBeamEnergy << '\t' << fBeamEnergy << G4endl;
  }

  // Beam NTuples
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
}

void MMRunData::Reset() {
  fBeamEnergy = 0.;
  fBeamCharge = 0;
  fBeamMass = 0;

  fEnergy = 0.;
  fCMEnergy = 0.;
  fVertexZ = 0.;

  fLightAngleCM = 0.;
  fLightAngleLab = 0.;
  fLightEnergy = 0.;
  fHeavyAngleCM = 0.;
  fHeavyAngleLab = 0.;
  fHeavyEnergy = 0.;

  fQValue = 0.;
  fExcitedEnergy = 0.;

  fLightRecoilCharge = -1;
  fLightRecoilMass = -1;
  fHeavyRecoilCharge = -1;
  fHeavyRecoilMass = -1;
}