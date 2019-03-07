#include "MMGenHit.hh"

G4ThreadLocal G4Allocator<MMGenHit>* MMGenHitAllocator;

MMGenHit::MMGenHit(G4int ID, G4double time) :
  G4VHit(), fID(ID), fTotalEnergy(0.), fFirstHitTime(time), fPreEnergy(0.),
  fPostEnergy(0.), fMass(0), fCharge(0), fXPosition(0.), fYPosition(0.), fZPosition(0.)
  {}

MMGenHit::~MMGenHit() {}
