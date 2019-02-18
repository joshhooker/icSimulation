#ifndef MMGenHit_h
#define MMGenHit_h

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"

#include "globals.hh"

class MMGenHit : public G4VHit {
public:
  MMGenHit(G4int,G4double);
  virtual ~MMGenHit();

  G4int GetID() const {return fID;};
  G4double GetFirstHitTime() const {return fFirstHitTime;};
  G4double GetTotalEnergy() const {return fTotalEnergy;};
  G4double GetPreEnergy() const {return fPreEnergy;};
  G4double GetPostEnergy() const {return fPostEnergy;};
  G4int GetMass() const {return fMass;};
  G4int GetCharge() const {return fCharge;};
  G4double GetPosition() const {return fPosition;};
  G4double GetHeight() const {return fHeight;};

  void SetFirstHitTime(G4double time) {fFirstHitTime = time;};
  void AddEnergy(G4double energy) {fTotalEnergy += energy;};
  void SetPreEnergy(G4double energy) {fPreEnergy = energy;};
  void SetPostEnergy(G4double energy) {fPostEnergy = energy;};
  void SetMass(G4int mass) {fMass = mass;};
  void SetCharge(G4int charge) {fCharge = charge;};
  void GetPositionFirstHit(G4double position) {fPosition = position;};
  void GetHeightFirstHit(G4double height) {fHeight = height;};

  inline void* operator new(size_t);
  inline void operator delete(void*);

private:
  G4int fID;
  G4double fTotalEnergy;
  G4double fFirstHitTime;
  G4double fPreEnergy;
  G4double fPostEnergy;
  G4int fMass;
  G4int fCharge;
  G4double fPosition;
  G4double fHeight;
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