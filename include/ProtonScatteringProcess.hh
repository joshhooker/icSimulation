#ifndef ProtonScatteringProcess_h
#define ProtonScatteringProcess_h

#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4VDiscreteProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"

#include "MMAnalysis.hh"
#include "MMDetectorConstruction.hh"
#include "MMEventAction.hh"
#include "MMTrackingInformation.hh"

#include "globals.hh"

class MMEventAction;

class ProtonScatteringProcess : public G4VDiscreteProcess {
public:
  ProtonScatteringProcess(const G4String& name = "ProtonScattering");
  ~ProtonScatteringProcess();

  G4double GetMeanFreePath(const G4Track&,G4double,
			   G4ForceCondition*);
  G4VParticleChange* PostStepDoIt(const G4Track&,const G4Step&);

  void StartTracking(G4Track*);

  void SetCMScatteringEnergy(G4double);

private:
  G4double fScatteringEnergy;
  G4double fCMScatteringEnergy;
  G4LogicalVolume* fScoringVolume;
  G4LogicalVolume* fScintLogical;
  G4LogicalVolume* fHavarLogical;
  G4LogicalVolume* fW1Logical;
  G4LogicalVolume* fW4Logical;
  G4LogicalVolume* fW7Logical;
};

#endif
