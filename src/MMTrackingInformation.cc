#include "MMTrackingInformation.hh"

MMTrackingInformation::MMTrackingInformation(G4double cmEnergy, G4double cmLightTheta,
                                             G4double labLightTheta, G4double cmLightPhi,
                                             G4double labAngle, G4double lightEnergy,
                                             G4double heavyEnergy, G4ThreeVector vertex) :
  fCMEnergy(cmEnergy), fCMLightTheta(cmLightTheta), fLabLightTheta(labLightTheta),
  fCMLightPhi(cmLightPhi), fLightAngle(labAngle), fLightEnergy(lightEnergy),
  fHeavyEnergy(heavyEnergy), fVertex(vertex) {

}

MMTrackingInformation::~MMTrackingInformation() {

}

void MMTrackingInformation::Print() const {

}
