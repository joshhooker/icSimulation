#ifndef MMGenSD_h
#define MMGenSD_h

#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4VSensitiveDetector.hh"

#include "MMGenHit.hh"

class MMGenSD : public G4VSensitiveDetector {
public:
  MMGenSD(G4String name);
  virtual ~MMGenSD();

  virtual void Initialize(G4HCofThisEvent* HCE);
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);

private:
  MMGenHitsCollection* fHitsCollection;
  G4int fHCID;
};

#endif
