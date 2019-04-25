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

  void SetScintXPos(std::vector<G4double>);
  void SetScintYPos(std::vector<G4double>);
  void SetScintZPos(std::vector<G4double>);
  void SetScintE(std::vector<G4double>);
  void SetScintT(std::vector<G4double>);
  void SetScintTrackID(std::vector<G4int>);
  void SetScintMass(std::vector<G4int>);
  void SetScintCharge(std::vector<G4int>);

  void SetEnergy(G4double);
  void SetCMEnergy(G4double);

  void SetCMLightAngle(G4double);
  void SetLabLightAngle(G4double);
  void SetLightEnergy(G4double);
  void SetCMHeavyAngle(G4double);
  void SetLabHeavyAngle(G4double);
  void SetHeavyEnergy(G4double);

  G4int GetReactionType() const {return fReactionType;}
  void SetReactionType(G4int);

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

  std::vector<G4double> fScintXPos;
  std::vector<G4double> fScintYPos;
  std::vector<G4double> fScintZPos;
  std::vector<G4double> fScintE;
  std::vector<G4double> fScintT;
  std::vector<G4int> fScintTrackID;
  std::vector<G4int> fScintCharge;
  std::vector<G4int> fScintMass;

  G4double fEnergy;
  G4double fCMEnergy;

  G4double fCMLightAngle;
  G4double fLabLightAngle;
  G4double fLightEnergy;
  G4double fCMHeavyAngle;
  G4double fLabHeavyAngle;
  G4double fHeavyEnergy;

  G4int fReactionType;
};

#endif
