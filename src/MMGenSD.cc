#include "MMGenSD.hh"
#include "G4SDManager.hh"

MMGenSD::MMGenSD(G4String name) :
  G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1) {
  G4String HCname = "genCollection";
  collectionName.insert(HCname);
}

MMGenSD::~MMGenSD() {}

void MMGenSD::Initialize(G4HCofThisEvent* hce) {
    fHitsCollection = new MMGenHitsCollection(SensitiveDetectorName,collectionName[0]);
    if (fHCID<0) {
      fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    }
    hce->AddHitsCollection(fHCID,fHitsCollection);
}

G4bool MMGenSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
  G4double edep = step->GetTotalEnergyDeposit();
  G4int hitMass = step->GetTrack()->GetDefinition()->GetAtomicMass();
  G4int hitCharge = step->GetTrack()->GetDefinition()->GetAtomicNumber();

  if (edep==0.) return true;

  G4StepPoint* preStepPoint = step->GetPreStepPoint();

  G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
  G4double posX = pos.x();
  G4double posY = pos.y();
  G4double posZ = pos.z();

  G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4int copyNo = touchable->GetVolume()->GetCopyNo();
  G4double hitTime = preStepPoint->GetGlobalTime();

  G4int ix = -1;
  for(G4int i = 0; i < fHitsCollection->entries(); i++) {
    if((*fHitsCollection)[i]->GetID()==copyNo) {
      ix = i;
      break;
    }
  }
  if(ix >= 0) {
    MMGenHit* hit = (*fHitsCollection)[ix];
    if(hit->GetFirstHitTime() > hitTime) {
      hit->SetFirstHitTime(hitTime);
    }
    hit->AddEnergy(edep);
    hit->SetMass(hitMass);
    hit->SetCharge(hitCharge);
  } else {
    MMGenHit* hit = new MMGenHit(copyNo, hitTime);
    hit->AddEnergy(edep);
    hit->SetMass(hitMass);
    hit->SetCharge(hitCharge);
    hit->SetXPosition(posX);
    hit->SetYPosition(posY);
    hit->SetZPosition(posZ);
    fHitsCollection->insert(hit);
  }
  return true;
}
