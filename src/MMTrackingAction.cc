#include "MMTrackingAction.hh"

MMTrackingAction::MMTrackingAction(MMEventAction* eventAction)
    : fEventAction(eventAction) {
    fName = "BinaryReaction";
}

MMTrackingAction::~MMTrackingAction() {}

void MMTrackingAction::PreUserTrackingAction(const G4Track* track) {
    const G4VProcess* creatorProcess = track->GetCreatorProcess();
    if(!creatorProcess) return;

    if(creatorProcess->GetProcessName() != fName) return;

    if(track->GetTrackID() != 2) return;

    MMTrackingInformation* info = (MMTrackingInformation*) track->GetUserInformation();

    fEventAction->SetEnergy(info->GetEnergy());
    fEventAction->SetCMEnergy(info->GetCMEnergy());
    fEventAction->SetVertexZ(info->GetVertex().z());
    fEventAction->SetQValue(info->GetQValue());
    fEventAction->SetExcitedEnergy(info->GetExcitedEnergy());

    fEventAction->SetLightAngleCM(info->GetCMLightTheta());
    fEventAction->SetLightAngleLab(info->GetLabLightTheta());
    fEventAction->SetLightEnergy(info->GetLightEnergy());
    fEventAction->SetLightRecoilCharge(info->GetLightRecoil()->GetAtomicNumber());
    fEventAction->SetLightRecoilMass(info->GetLightRecoil()->GetAtomicMass());

    fEventAction->SetHeavyAngleCM(info->GetCMHeavyTheta());
    fEventAction->SetHeavyAngleLab(info->GetLabHeavyTheta());
    fEventAction->SetHeavyEnergy(info->GetHeavyEnergy());
    fEventAction->SetHeavyRecoilCharge(info->GetHeavyRecoil()->GetAtomicNumber());
    fEventAction->SetHeavyRecoilMass(info->GetHeavyRecoil()->GetAtomicMass());
}
