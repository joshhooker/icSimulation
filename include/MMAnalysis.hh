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
  void SetFilename(const char*);

  void OpenFile();
  void CloseFile();

  void SetGridEnergy(std::vector<G4double>);
  void SetGridID(std::vector<G4int>);
  void SetGridTrackID(std::vector<G4int>);

  void SetScintXPosition(std::vector<G4double>);
  void SetScintYPosition(std::vector<G4double>);
  void SetScintZPosition(std::vector<G4double>);
  void SetScintEnergy(std::vector<G4double>);
  void SetScintTime(std::vector<G4double>);
  void SetScintTrackID(std::vector<G4int>);
  void SetScintCharge(std::vector<G4int>);
  void SetScintMass(std::vector<G4int>);

  void SetEnergy(G4double);
  void SetCMEnergy(G4double);

  void SetBeamEnergy(G4double);
  void SetBeamCharge(G4int);
  void SetBeamMass(G4int);

  void SetCMLightAngle(G4double);
  void SetLabLightAngle(G4double);
  void SetLightEnergy(G4double);
  void SetCMHeavyAngle(G4double);
  void SetLabHeavyAngle(G4double);
  void SetHeavyEnergy(G4double);

  void Fill();
  void FillAll();

private:
  static MMAnalysis* fInstance;
  std::string fFilename;
  TFile* fFile;
  TTree* fTree;
  TTree* fAllTree;

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

};

#endif
