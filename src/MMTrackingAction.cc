#include "MMTrackingAction.hh"

MMTrackingAction::MMTrackingAction() {
  fName = "BinaryReaction";
}

MMTrackingAction::~MMTrackingAction() {}

void MMTrackingAction::PreUserTrackingAction(const G4Track* track) {
  const G4VProcess* creatorProcess = track->GetCreatorProcess();
  if(!creatorProcess) return;

  if(creatorProcess->GetProcessName() != fName) return;

  if(track->GetTrackID() != 2) return;

  MMTrackingInformation* info = (MMTrackingInformation*) track->GetUserInformation();

  MMAnalysis* analysis = MMAnalysis::Instance();
  analysis->SetEnergy(info->GetEnergy());
  analysis->SetCMEnergy(info->GetCMEnergy());
  analysis->SetCMLightAngle(info->GetCMLightTheta());
  analysis->SetLabLightAngle(info->GetLabLightTheta());
  analysis->SetLightEnergy(info->GetLightEnergy());
  analysis->SetCMHeavyAngle(info->GetCMHeavyTheta());
  analysis->SetLabHeavyAngle(info->GetLabHeavyTheta());
  analysis->SetHeavyEnergy(info->GetHeavyEnergy());
  analysis->SetQValue(info->GetQValue());
  analysis->SetLightRecoilCharge(info->GetLightRecoil()->GetAtomicNumber());
  analysis->SetLightRecoilMass(info->GetLightRecoil()->GetAtomicMass());
  analysis->SetHeavyRecoilCharge(info->GetHeavyRecoil()->GetAtomicNumber());
  analysis->SetHeavyRecoilMass(info->GetHeavyRecoil()->GetAtomicMass());
}
