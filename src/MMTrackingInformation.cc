#include "MMTrackingInformation.hh"

MMTrackingInformation::MMTrackingInformation(G4double energy, G4double cmEnergy, G4double cmLightTheta,
                                             G4double labLightTheta, G4double cmLightPhi, G4double cmHeavyTheta,
                                             G4double labHeavyTheta, G4double lightEnergy, G4double heavyEnergy,
                                             G4ThreeVector vertex, G4double qValue, G4double excitedEnergy,
                                             G4ParticleDefinition* lightRecoil, G4ParticleDefinition* heavyRecoil) {
  fEnergy = energy;
  fCMEnergy = cmEnergy;
  fCMLightTheta = cmLightTheta;
  fLabLightTheta = labLightTheta;
  fCMLightPhi = cmLightPhi;
  fCMHeavyTheta = cmHeavyTheta;
  fLabHeavyTheta = labHeavyTheta;
  fLightEnergy = lightEnergy;
  fHeavyEnergy = heavyEnergy;
  fVertex = vertex;
  fQValue = qValue;
  fExcitedEnergy = excitedEnergy;
  fLightRecoil = lightRecoil;
  fHeavyRecoil = heavyRecoil;
}

MMTrackingInformation::~MMTrackingInformation() {}

void MMTrackingInformation::Print() const {}
