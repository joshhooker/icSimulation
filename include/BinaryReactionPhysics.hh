#ifndef BinaryReactionPhysics_h
#define BinaryReactionPhysics_h

#include "G4GenericIon.hh"
#include "G4PhysicsListHelper.hh"
#include "G4VPhysicsConstructor.hh"

#include "globals.hh"

#include "BinaryReactionProcess.hh"

#include <string>
#include <map>

class BinaryReactionPhysics : public G4VPhysicsConstructor {
public:
  BinaryReactionPhysics(G4int verbose = 1);
  BinaryReactionPhysics(const G4String& name);
  virtual ~BinaryReactionPhysics();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

  void SetQValue(G4double qValue) {
    fQValue = qValue;
  }

  void SetTarget(G4int charge, G4int mass) {
    fTargetCharge = charge;
    fTargetMass = mass;
  }

  void SetLightProduct(G4int charge, G4int mass) {
    fLightProductCharge = charge;
    fLightProductMass = mass;
  }

  void SetHeavyProduct(G4int charge, G4int mass) {
    fHeavyProductCharge = charge;
    fHeavyProductMass = mass;
  }

  void SetNumGrids(G4int numGrids) {
    fNumGrids = numGrids;
  }

private:
  G4double fQValue;
  G4int fLightProductCharge;
  G4int fLightProductMass;
  G4int fHeavyProductCharge;
  G4int fHeavyProductMass;
  G4int fTargetCharge;
  G4int fTargetMass;
  G4int fNumGrids;
};

#endif
