#ifndef MMRunAction_h
#define MMRunAction_h

#include "G4EmCalculator.hh"
#include "G4ParticleGun.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"
#include "G4UnitsTable.hh"
#include "G4UserRunAction.hh"

#include "MMAnalysis.hh"
#include "MMDetectorConstruction.hh"
#include "MMEventAction.hh"
#include "MMPrimaryGeneratorAction.hh"
#include "MMRunData.hh"

class MMDetectorConstruction;
class MMPrimaryGeneratorAction;

class MMRunAction : public G4UserRunAction {
public:
  MMRunAction(MMDetectorConstruction*, MMPrimaryGeneratorAction*);
  ~MMRunAction();

  virtual G4Run* GenerateRun();

  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

private:
  MMDetectorConstruction* fDetector;
  MMPrimaryGeneratorAction* fPrimary;

  std::vector<G4double> fGridEnergy;
};

#endif
