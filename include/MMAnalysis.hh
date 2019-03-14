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

  void SetGunEnergy(G4double);

  void SetICGridEnergy(std::vector<G4double>& v);
  void SetICGridTotalEnergy(G4double);

  void SetScintE(G4double);
  void SetScintMass(G4int);
  void SetScintCharge(G4int);
  void SetScintXPos(G4double);
  void SetScintYPos(G4double);
  void SetScintZPos(G4double);

  void SetEnergy(G4double);
  void SetCMEnergy(G4double);

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

  G4double fGunEnergy;

  std::vector<G4double> fICGridEnergy;
  G4double fICGridTotalEnergy;

  G4double fScintE;
  G4int fScintMass;
  G4int fScintCharge;
  G4double fScintXPos;
  G4double fScintYPos;
  G4double fScintZPos;

  G4double fEnergy;
  G4double fCMEnergy;

  G4double fCMLightAngle;
  G4double fLabLightAngle;
  G4double fLightEnergy;
  G4double fCMHeavyAngle;
  G4double fLabHeavyAngle;
  G4double fHeavyEnergy;
};

#endif
