#ifndef MMTrackingInformation_h
#define MMTrackingInformation_h

#include "G4ThreeVector.hh"
#include "G4VUserTrackInformation.hh"

class MMTrackingInformation : public G4VUserTrackInformation {

public:
  MMTrackingInformation(G4double, G4double, G4double, G4double, G4ThreeVector);
  ~MMTrackingInformation();

  void Print() const;

  void SetCMEnergy(G4double cmEnergy) {fCMEnergy = cmEnergy;};
  void SetCMLightTheta(G4double theta) {fCMLightTheta = theta;};
  void SetLabLightTheta(G4double theta) {fLabLightTheta = theta;};
  void SetCMLightPhi(G4double phi) {fCMLightPhi = phi;};
  G4double GetCMEnergy() const {return fCMEnergy;};
  G4double GetCMLightTheta() const {return fCMLightTheta;};
  G4double GetLabLightTheta() const {return fLabLightTheta;};
  G4double GetCMLightPhi() const {return fCMLightPhi;};
  G4ThreeVector GetVertex() const {return fVertex;};

private:
  G4double fCMEnergy;
  G4double fCMLightTheta;
  G4double fLabLightTheta;
  G4double fCMLightPhi;
  G4ThreeVector fVertex;
};

#endif
