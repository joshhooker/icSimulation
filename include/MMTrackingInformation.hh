#ifndef MMTrackingInformation_h
#define MMTrackingInformation_h

#include "G4ThreeVector.hh"
#include "G4VUserTrackInformation.hh"

class MMTrackingInformation : public G4VUserTrackInformation {

public:
  MMTrackingInformation(G4double, G4double, G4double, G4double, G4double,
                        G4double, G4double, G4double, G4ThreeVector);
  ~MMTrackingInformation();

  void Print() const;

  G4double GetCMEnergy() const {return fCMEnergy;}
  G4double GetCMLightTheta() const {return fCMLightTheta;}
  G4double GetLabLightTheta() const {return fLabLightTheta;}
  G4double GetCMLightPhi() const {return fCMLightPhi;}
  G4double GetLightAngleLab() const {return fLightAngle;}
  G4double GetLightEnergyLab() const {return fLightEnergy;}
  G4double GetHeavyAngleLab() const {return fHeavyAngle;}
  G4double GetHeavyEnergyLab() const {return fHeavyEnergy;}
  G4ThreeVector GetVertex() const {return fVertex;}

  void SetCMEnergy(G4double cmEnergy) {fCMEnergy = cmEnergy;}
  void SetCMLightTheta(G4double theta) {fCMLightTheta = theta;}
  void SetLabLightTheta(G4double theta) {fLabLightTheta = theta;}
  void SetCMLightPhi(G4double phi) {fCMLightPhi = phi;}
  void SetLightAngleLab(G4double angle) {fLightAngle = angle;}
  void SetLightEnergyLab(G4double energy) {fLightEnergy = energy;}
  void SetHeavyEnergyLab(G4double energy) {fHeavyEnergy = energy;}
  void SetVertex(G4ThreeVector v) {fVertex = v;}

private:
  G4double fCMEnergy;
  G4double fCMLightTheta;
  G4double fLabLightTheta;
  G4double fCMLightPhi;
  G4double fLightAngle;
  G4double fLightEnergy;
  G4double fHeavyAngle;
  G4double fHeavyEnergy;
  G4ThreeVector fVertex;
};

#endif
