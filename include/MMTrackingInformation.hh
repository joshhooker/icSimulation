#ifndef MMTrackingInformation_h
#define MMTrackingInformation_h

#include "G4ThreeVector.hh"
#include "G4VUserTrackInformation.hh"

class MMTrackingInformation : public G4VUserTrackInformation {

public:
  MMTrackingInformation(G4double,G4double,G4double,G4ThreeVector);
  ~MMTrackingInformation();

  void Print() const;

private:
};

#endif
