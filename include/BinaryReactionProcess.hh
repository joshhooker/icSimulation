#ifndef BinaryReactionProcess_h
#define BinaryReactionProcess_h

#include "G4IonTable.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VDiscreteProcess.hh"

#include "MMDetectorConstruction.hh"
#include "MMTrackingInformation.hh"

class BinaryReactionProcess : public G4VDiscreteProcess {
public:
  BinaryReactionProcess(const G4String& name = "BinaryReaction");
  ~BinaryReactionProcess();

  G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition*);
  G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

  void StartTracking(G4Track*);
  void SetQValue(G4double qValue) {
    fQValue = qValue;
    G4cout << "SET: Q Value: " << fQValue << G4endl;
  }
  void SetTarget(G4int charge, G4int mass) {
    fTargetMass = mass;
    fTargetCharge = charge;
    G4cout << "SET: Target: " << fTargetCharge << ' ' << fTargetMass << G4endl;
  }
  void SetLightProduct(G4int charge, G4int mass) {
    fLightProductMass = mass;
    fLightProductCharge = charge;
    G4cout << "SET: Light Product: " << fLightProductCharge << ' ' << fLightProductMass << G4endl;
  }
  void SetHeavyProduct(G4int charge, G4int mass) {
    fHeavyProductMass = mass;
    fHeavyProductCharge = charge;
    G4cout << "SET: Heavy Product: " << fHeavyProductCharge << ' ' << fHeavyProductMass << G4endl;
  }
  G4double GetLightProductMass() {
    return fLightProductMass;
  }
  G4double GetLightProductCharge() {
    return fLightProductCharge;
  }
  void ParseParams(std::map<std::string, G4int>&);

private:
  G4double fScatteringEnergy;
  G4double fQValue;

  G4int fTargetMass;
  G4int fTargetCharge;
  G4int fLightProductMass;
  G4int fLightProductCharge;
  G4int fHeavyProductMass;
  G4int fHeavyProductCharge;
};

#endif
