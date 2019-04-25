#ifndef MMGenHit_h
#define MMGenHit_h

#include "G4Allocator.hh"
#include "G4ParticleDefinition.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"

#include "globals.hh"

class MMGenHit : public G4VHit {
public:
  MMGenHit(G4int, G4int, G4double, G4double, G4ThreeVector, G4ParticleDefinition*);
  virtual ~MMGenHit();

  G4int GetID() const {return fID;}
  G4int GetTrackID() const {return fTrackID;}
  G4double GetTime() const {return fTime;}
  G4double GetEnergy() const {return fEnergy;}
  G4ThreeVector GetPosition() const {return fPosition;}
  G4ParticleDefinition* GetParticle() const {return fParticle;}

  inline void* operator new(size_t);
  inline void operator delete(void*);

private:
  G4int fID;
  G4int fTrackID;
  G4double fTime;
  G4double fEnergy;
  G4ThreeVector fPosition;
  G4ParticleDefinition* fParticle;
};

typedef G4THitsCollection<MMGenHit> MMGenHitsCollection;

extern G4ThreadLocal G4Allocator<MMGenHit>* MMGenHitAllocator;

inline void* MMGenHit::operator new(size_t)
{
    if (!MMGenHitAllocator) MMGenHitAllocator = new G4Allocator<MMGenHit>;
    return (void*)MMGenHitAllocator->MallocSingle();
}

inline void MMGenHit::operator delete(void* aHit)
{
    MMGenHitAllocator->FreeSingle((MMGenHit*) aHit);
}

#endif
