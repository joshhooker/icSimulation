#ifndef MMTrackingAction_h
#define MMTrackingAction_h

#include "G4String.hh"
#include "G4UserTrackingAction.hh"
#include "G4VProcess.hh"

#include "MMTrackingInformation.hh"

class MMTrackingAction : public G4UserTrackingAction {
public:
  MMTrackingAction();
  ~MMTrackingAction();

  void PreUserTrackingAction(const G4Track*);

private:
  G4String fName;
};

#endif
