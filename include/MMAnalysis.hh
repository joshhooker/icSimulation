#ifndef MMAnalysis_h
#define MMAnalysis_h

#include "G4ThreeVector.hh"

#include <TFile.h>
#include <TTree.h>

#include <vector>
#include <string>

class TFile;
class TTree;

class MMAnalysis {
public:
  MMAnalysis();
  static MMAnalysis* Instance();
  void SetFilename(const char* filename) {fFilename = filename;}

  void OpenFile();
  void CloseFile() {
    fTree->Write();
    fFile->Close();
  }

  void SetGridEnergy(std::vector<G4double> energy) {fGridEnergy = energy;}
  void SetGridID(std::vector<G4int> id) {fGridID = id;}
  void SetGridTrackID(std::vector<G4int> trackID) {fGridTrackID = trackID;}

  void SetScintXPosition(std::vector<G4double> position) {fScintXPosition = position;}
  void SetScintYPosition(std::vector<G4double> position) {fScintYPosition = position;}
  void SetScintZPosition(std::vector<G4double> position) {fScintZPosition = position;}
  void SetScintEnergy(std::vector<G4double> energy) {fScintEnergy = energy;}
  void SetScintTime(std::vector<G4double> time) {fScintTime = time;}
  void SetScintTrackID(std::vector<G4int> trackID) {fScintTrackID = trackID;}
  void SetScintCharge(std::vector<G4int> charge) {fScintCharge = charge;}
  void SetScintMass(std::vector<G4int> mass) {fScintMass = mass;}

  void SetEnergy(G4double energy) {fEnergy = energy;}
  void SetCMEnergy(G4double energy) {fCMEnergy = energy;}

  void SetBeamEnergy(G4double energy) {fBeamEnergy = energy;}
  void SetBeamCharge(G4int charge) {fBeamCharge = charge;}
  void SetBeamMass(G4int mass) {fBeamMass = mass;}

  void SetCMLightAngle(G4double angle) {fCMLightAngle = angle;}
  void SetLabLightAngle(G4double angle) {fLabLightAngle = angle;}
  void SetLightEnergy(G4double energy) {fLightEnergy = energy;}
  void SetCMHeavyAngle(G4double angle) {fCMHeavyAngle = angle;}
  void SetLabHeavyAngle(G4double angle) {fLabHeavyAngle = angle;}
  void SetHeavyEnergy(G4double energy) {fHeavyEnergy = energy;}
  void SetQValue(G4double qValue) {fQValue = qValue;}
  void SetLightRecoilCharge(G4int charge) {fLightRecoilCharge = charge;}
  void SetLightRecoilMass(G4int mass) {fLightRecoilMass = mass;}
  void SetHeavyRecoilCharge(G4int charge) {fHeavyRecoilCharge = charge;}
  void SetHeavyRecoilMass(G4int mass) {fHeavyRecoilMass = mass;}

  void Fill() {fTree->Fill();}

private:
  static MMAnalysis* fInstance;
  std::string fFilename;
  TFile* fFile;
  TTree* fTree;

  std::vector<G4double> fGridEnergy;
  std::vector<G4int> fGridID;
  std::vector<G4int> fGridTrackID;

  std::vector<G4double> fScintXPosition;
  std::vector<G4double> fScintYPosition;
  std::vector<G4double> fScintZPosition;
  std::vector<G4double> fScintEnergy;
  std::vector<G4double> fScintTime;
  std::vector<G4int> fScintTrackID;
  std::vector<G4int> fScintCharge;
  std::vector<G4int> fScintMass;

  G4double fEnergy;
  G4double fCMEnergy;

  G4double fBeamEnergy;
  G4int fBeamCharge;
  G4int fBeamMass;

  G4double fCMLightAngle;
  G4double fLabLightAngle;
  G4double fLightEnergy;
  G4double fCMHeavyAngle;
  G4double fLabHeavyAngle;
  G4double fHeavyEnergy;
  G4double fQValue;
  G4int fLightRecoilCharge;
  G4int fLightRecoilMass;
  G4int fHeavyRecoilCharge;
  G4int fHeavyRecoilMass;

};

#endif
