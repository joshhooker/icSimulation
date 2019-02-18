#include "MMActionInitialization.hh"

MMActionInitialization::MMActionInitialization() :
  G4VUserActionInitialization() {

}

MMActionInitialization::~MMActionInitialization() {
}

void MMActionInitialization::BuildForMaster() const {
  SetUserAction(new MMRunAction);
}

void MMActionInitialization::Build() const {
  MMPrimaryGeneratorAction* primary = new MMPrimaryGeneratorAction();
  SetUserAction(primary);

  SetUserAction(new MMRunAction());

  MMEventAction* eventAction = new MMEventAction();
  SetUserAction(eventAction);

  SetUserAction(new MMTrackingAction);
}
