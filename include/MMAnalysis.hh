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

  void SetGunEnergy(double);

  void SetICGridEnergy(std::vector<G4double>& v);
  void SetICGridTotalEnergy(double);

  void SetScintE(double);
  void SetScintMass(int);
  void SetScintCharge(int);
  void SetScintXPos(double);
  void SetScintYPos(double);
  void SetScintZPos(double);

  void SetLightAngleLab(double);
  void SetLightEnergyLab(double);
  void SetHeavyAngleLab(double);
  void SetHeavyEnergyLab(double);

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

  G4double fLightAngleLab;
  G4double fLightEnergyLab;
  G4double fpLightAngleCM;
  G4double faLightAngleCM;
  G4double fLightAngleCM;
  G4double fHeavyAngleLab;
  G4double fHeavyEnergyLab;
  G4double fHeavyAngleCM;
};

#endif
