#ifndef MMTrackingInformation_h
#define MMTrackingInformation_h

#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4VUserTrackInformation.hh"

class MMTrackingInformation : public G4VUserTrackInformation {

public:
  MMTrackingInformation(G4double, G4double, G4double, G4double, G4double,
                        G4double, G4double, G4double, G4double, G4ThreeVector,
                        G4double, G4double, G4ParticleDefinition*,
                        G4ParticleDefinition*);
  ~MMTrackingInformation();

  void Print() const;

  G4double GetEnergy() const {return fEnergy;}
  G4double GetCMEnergy() const {return fCMEnergy;}
  G4double GetCMLightTheta() const {return fCMLightTheta;}
  G4double GetLabLightTheta() const {return fLabLightTheta;}
  G4double GetCMLightPhi() const {return fCMLightPhi;}
  G4double GetCMHeavyTheta() const {return fCMHeavyTheta;}
  G4double GetLabHeavyTheta() const {return fLabHeavyTheta;}
  G4double GetLightEnergy() const {return fLightEnergy;}
  G4double GetHeavyEnergy() const {return fHeavyEnergy;}
  G4ThreeVector GetVertex() const {return fVertex;}
  G4double GetQValue() const {return fQValue;}
  G4double GetExcitedEnergy() const {return fExcitedEnergy;}
  G4ParticleDefinition* GetLightRecoil() const {return fLightRecoil;}
  G4ParticleDefinition* GetHeavyRecoil() const {return fHeavyRecoil;}

  void SetEnergy(G4double energy) {fEnergy = energy;}
  void SetCMEnergy(G4double cmEnergy) {fCMEnergy = cmEnergy;}
  void SetCMLightTheta(G4double theta) {fCMLightTheta = theta;}
  void SetLabLightTheta(G4double theta) {fLabLightTheta = theta;}
  void SetCMLightPhi(G4double phi) {fCMLightPhi = phi;}
  void SetCMHeavyTheta(G4double angle) {fCMHeavyTheta = angle;}
  void SetLabHeavyTheta(G4double energy) {fLabHeavyTheta = energy;}
  void SetLightEnergy(G4double energy) {fLightEnergy = energy;}
  void SetHeavyEnergy(G4double energy) {fHeavyEnergy = energy;}
  void SetVertex(G4ThreeVector v) {fVertex = v;}
  void SetQValue(G4double qValue) {fQValue = qValue;}
  void SetExcitedEnergy(G4double excitedEnergy) {fExcitedEnergy = excitedEnergy;}
  void SetLightRecoil(G4ParticleDefinition* particle) {fLightRecoil = particle;}
  void SetHeavyRecoil(G4ParticleDefinition* particle) {fHeavyRecoil = particle;}

private:
  G4double fEnergy;
  G4double fCMEnergy;
  G4double fCMLightTheta;
  G4double fLabLightTheta;
  G4double fCMLightPhi;
  G4double fCMHeavyTheta;
  G4double fLabHeavyTheta;
  G4double fLightEnergy;
  G4double fHeavyEnergy;
  G4ThreeVector fVertex;
  G4double fQValue;
  G4double fExcitedEnergy;
  G4ParticleDefinition* fLightRecoil;
  G4ParticleDefinition* fHeavyRecoil;
};

#endif
