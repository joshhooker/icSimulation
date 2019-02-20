#include "MMTrackingInformation.hh"

MMTrackingInformation::MMTrackingInformation(G4double cmEnergy, G4double cmLightTheta, G4double labLightTheta, G4double cmLightPhi, G4ThreeVector vertex) : fCMEnergy(cmEnergy), fCMLightTheta(cmLightTheta), fLabLightTheta(labLightTheta), fCMLightPhi(cmLightPhi), fVertex(vertex) {

}

MMTrackingInformation::~MMTrackingInformation() {

}

void MMTrackingInformation::Print() const {

}
