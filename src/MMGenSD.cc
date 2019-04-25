#include "MMGenSD.hh"
#include "G4SDManager.hh"

MMGenSD::MMGenSD(G4String name) :
  G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1) {
  G4String HCname = "genCollection";
  collectionName.insert(HCname);
}

MMGenSD::~MMGenSD() {}

void MMGenSD::Initialize(G4HCofThisEvent* hce) {
  fHitsCollection = new MMGenHitsCollection(SensitiveDetectorName, collectionName[0]);
  if(fHCID < 0) {
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }
  hce->AddHitsCollection(fHCID, fHitsCollection);
}

G4bool MMGenSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
  G4double edep = step->GetTotalEnergyDeposit();

  if(edep/eV < .1) return true;

  G4String type = step->GetTrack()->GetDefinition()->GetParticleType();
  if(type != "nucleus" && type != "baryon") return true;

  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4StepPoint* postStepPoint = step->GetPostStepPoint();

  G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4int copyNo = touchable->GetVolume()->GetCopyNo();
  G4double hitTime = preStepPoint->GetGlobalTime();
  G4int trackID = step->GetTrack()->GetTrackID();

  G4ThreeVector position = preStepPoint->GetPosition();

  G4ParticleDefinition* particle = step->GetTrack()->GetDefinition();

  MMGenHit* hit = new MMGenHit(copyNo, trackID, hitTime/ns, edep/MeV, position/mm, particle);
  fHitsCollection->insert(hit);

  return true;
}
