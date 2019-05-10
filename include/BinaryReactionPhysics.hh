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

  void SetNumGrids(G4int numGrids) {fNumGrids = numGrids;}

private:
  G4int fNumGrids;
};

#endif
