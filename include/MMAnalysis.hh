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

  void SetICGrid1E(double);
  void SetICGrid2E(double);
  void SetICGrid3E(double);
  void SetICGrid4E(double);
  void SetICGridTotalE(double);

  void SetScintE(double);

  void Fill();

private:
  static MMAnalysis* fInstance;
  std::string fFilename;
  TFile* fFile;
  TTree* fTree;

  G4double fGunEnergy;

  G4double fICGrid1E;
  G4double fICGrid2E;
  G4double fICGrid3E;
  G4double fICGrid4E;
  G4double fICGridTotalE;

  G4double fScintE;
};

#endif
