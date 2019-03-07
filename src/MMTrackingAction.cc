#include "MMTrackingAction.hh"

MMTrackingAction::MMTrackingAction() {
  fName = "BinaryReaction";
}

MMTrackingAction::~MMTrackingAction() {
}

void MMTrackingAction::PreUserTrackingAction(const G4Track* track) {
  const G4VProcess* creatorProcess = track->GetCreatorProcess();
  if(!creatorProcess) return;

  if(creatorProcess->GetProcessName() != fName) return;

  BinaryReactionProcess* reactionProcess = (BinaryReactionProcess*) creatorProcess;
  if(track->GetParticleDefinition()->GetAtomicMass() != G4int(reactionProcess->GetLightProductMass()) ||
     track->GetParticleDefinition()->GetAtomicNumber() != G4int(reactionProcess->GetLightProductCharge())) return;

  MMTrackingInformation* info = (MMTrackingInformation*) track->GetUserInformation();

  MMAnalysis* analysis = MMAnalysis::Instance();
  analysis->SetLightAngleLab(info->GetLightAngleLab());
  analysis->SetLightEnergyLab(info->GetLightEnergyLab());
  analysis->SetHeavyEnergyLab(info->GetHeavyEnergyLab());
}
