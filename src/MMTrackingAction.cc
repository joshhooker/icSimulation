#include "MMTrackingAction.hh"

MMTrackingAction::MMTrackingAction() {
  fName = "ProtonScattering";
}

MMTrackingAction::~MMTrackingAction() {
}

void MMTrackingAction::PreUserTrackingAction(const G4Track* track) {
  const G4VProcess* creatorProcess = track->GetCreatorProcess();
  if(!creatorProcess) return;

  if(creatorProcess->GetProcessName() != fName) return;
//  if(track->GetParticleDefinition()->GetParticleName() != "proton") return;

  MMTrackingInformation* info = (MMTrackingInformation*) track->GetUserInformation();
}
