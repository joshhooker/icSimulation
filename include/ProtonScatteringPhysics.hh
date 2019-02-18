#ifndef ProtonScatteringPhysics_h
#define ProtonScatteringPhysics_h

#include "G4GenericIon.hh"
#include "G4PhysicsConstructorFactory.hh"
#include "G4PhysicsListHelper.hh"
#include "G4VPhysicsConstructor.hh"

#include "globals.hh"

#include "ProtonScatteringProcess.hh"

class ProtonScatteringPhysics : public G4VPhysicsConstructor {
public:
  ProtonScatteringPhysics(G4int verbose =1);
  ProtonScatteringPhysics(const G4String& name);
  void SetCMScatteringEnergy(G4double);
  virtual ~ProtonScatteringPhysics();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4double fCMScatteringEnergy;
};

#endif
