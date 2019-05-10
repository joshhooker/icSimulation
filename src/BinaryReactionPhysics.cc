#include "BinaryReactionPhysics.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(BinaryReactionPhysics);

BinaryReactionPhysics::BinaryReactionPhysics(G4int)
:  G4VPhysicsConstructor("BinaryReactionPhysics") {
}

BinaryReactionPhysics::BinaryReactionPhysics(const G4String& name)
:  G4VPhysicsConstructor(name) {
}

BinaryReactionPhysics::~BinaryReactionPhysics() {}

void BinaryReactionPhysics::ConstructParticle() {
  G4GenericIon::GenericIon();
}

void BinaryReactionPhysics::ConstructProcess() {
  BinaryReactionProcess* reactionProcess = new BinaryReactionProcess();
  reactionProcess->SetNumGrids(fNumGrids);
  G4PhysicsListHelper::GetPhysicsListHelper()->RegisterProcess(reactionProcess, G4GenericIon::GenericIon());
}

