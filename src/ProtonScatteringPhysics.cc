#include "ProtonScatteringPhysics.hh"

G4_DECLARE_PHYSCONSTR_FACTORY(ProtonScatteringPhysics);

ProtonScatteringPhysics::ProtonScatteringPhysics(G4int)
:   G4VPhysicsConstructor("G4RadioactiveDecay"), fCMScatteringEnergy(0.) {
}

ProtonScatteringPhysics::ProtonScatteringPhysics(const G4String& name)
:  G4VPhysicsConstructor(name) {
}

ProtonScatteringPhysics::~ProtonScatteringPhysics()
{
}

void ProtonScatteringPhysics::ConstructParticle()
{
  G4GenericIon::GenericIon();
}

void ProtonScatteringPhysics::ConstructProcess()
{
  ProtonScatteringProcess* protonScatteringProcess = new ProtonScatteringProcess;
  protonScatteringProcess->SetCMScatteringEnergy(fCMScatteringEnergy);
  G4PhysicsListHelper::GetPhysicsListHelper()->
    RegisterProcess(protonScatteringProcess, G4GenericIon::GenericIon());
}

void ProtonScatteringPhysics::SetCMScatteringEnergy(G4double energy) {
  fCMScatteringEnergy = energy;
}
