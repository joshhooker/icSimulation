#ifndef MMTrackingAction_h
#define MMTrackingAction_h

#include "G4String.hh"
#include "G4UserTrackingAction.hh"
#include "G4VProcess.hh"

#include "BinaryReactionProcess.hh"
#include "MMEventAction.hh"
#include "MMTrackingInformation.hh"

class MMTrackingAction : public G4UserTrackingAction {
public:
    MMTrackingAction(MMEventAction*);
    ~MMTrackingAction();

    void PreUserTrackingAction(const G4Track*);

private:
    G4String fName;
    MMEventAction* fEventAction;
};

#endif
