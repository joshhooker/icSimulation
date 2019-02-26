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

  void Fill();

private:
  static MMAnalysis* fInstance;
  std::string fFilename;
  TFile* fFile;
  TTree* fTree;

  G4double fGunEnergy;

  std::vector<G4double> fICGridEnergy;
  G4double fICGridTotalEnergy;

  G4double fScintE;
};

#endif
