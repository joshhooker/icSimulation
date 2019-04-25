#include "MMGenHit.hh"

G4ThreadLocal G4Allocator<MMGenHit>* MMGenHitAllocator;

MMGenHit::MMGenHit(G4int ID, G4int trackID, G4double time, G4double energy, G4ThreeVector pos, G4ParticleDefinition* particle) :
  G4VHit(), fID(ID), fTrackID(trackID), fTime(time), fEnergy(energy), fPosition(pos), fParticle(particle) {}

MMGenHit::~MMGenHit() {}
