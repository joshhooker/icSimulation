#include "MMActionInitialization.hh"

MMActionInitialization::MMActionInitialization(MMDetectorConstruction* detector) :
  G4VUserActionInitialization(), fDetector(detector) {

}

MMActionInitialization::~MMActionInitialization() {
}

void MMActionInitialization::BuildForMaster() const {
  SetUserAction(new MMRunAction(fDetector, NULL));
}

void MMActionInitialization::Build() const {
  MMPrimaryGeneratorAction* primary = new MMPrimaryGeneratorAction();
  SetUserAction(primary);

  SetUserAction(new MMRunAction(fDetector, primary));

  MMEventAction* eventAction = new MMEventAction(fNumGrids);
  eventAction->SetFanoFactor(fFanoFactor);
  eventAction->SetWorkFunction(fWorkFunction);
  eventAction->SetScintillatorResolution(fScintResolution);
  eventAction->SetGridResolution(fGridResolution);
  SetUserAction(eventAction);

  SetUserAction(new MMTrackingAction);
}
