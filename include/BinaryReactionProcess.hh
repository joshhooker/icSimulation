#ifndef BinaryReactionProcess_h
#define BinaryReactionProcess_h

#include "G4IonTable.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VDiscreteProcess.hh"

#include "MMAnalysis.hh"
#include "MMDetectorConstruction.hh"
#include "MMTrackingInformation.hh"

#include <cmath>

class BinaryReactionProcess : public G4VDiscreteProcess {
public:
  BinaryReactionProcess(const G4String& name = "BinaryReaction");
  ~BinaryReactionProcess();

  G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition*);
  G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

  G4VParticleChange* Decay(const G4Track&, G4int, G4int, G4int, G4int);

  void StartTracking(G4Track*);

  void SetNumGrids(G4int numGrids) {fNumGrids = numGrids;}

private:
  G4double fScatteringEnergy;

  G4int fNumGrids;
};

#endif
