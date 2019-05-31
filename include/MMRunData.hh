#ifndef MMRunData_h
#define MMRunData_h

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "globals.hh"

#include "MMAnalysis.hh"

class MMRunData : public G4Run {
public:
  MMRunData();
  static MMRunData* Instance();
  virtual ~MMRunData();

  void AddBeamEvent(G4double, G4int, G4int);
  void AddReactionEvent(G4double, G4double, G4double, G4double, G4double);
  void AddLightRecoil(G4double, G4double, G4double, G4int, G4int);
  void AddHeavyRecoil(G4double, G4double, G4double, G4int, G4int);
  void FillPerEvent();

  void Reset();

private:
  static MMRunData* fInstance;

  G4double fBeamEnergy;
  G4int fBeamCharge;
  G4int fBeamMass;

  G4double fEnergy;
  G4double fCMEnergy;
  G4double fVertexZ;

  G4double fLightAngleCM;
  G4double fLightAngleLab;
  G4double fLightEnergy;
  G4double fHeavyAngleCM;
  G4double fHeavyAngleLab;
  G4double fHeavyEnergy;

  G4double fQValue;
  G4double fExcitedEnergy;

  G4int fLightRecoilCharge;
  G4int fLightRecoilMass;
  G4int fHeavyRecoilCharge;
  G4int fHeavyRecoilMass;
};

inline void MMRunData::AddBeamEvent(G4double energy, G4int charge, G4int mass) {
  fBeamEnergy = energy;
  fBeamCharge = charge;
  fBeamMass = mass;
}

inline void MMRunData::AddReactionEvent(G4double energy, G4double cmEnergy, G4double vertexZ,
                                        G4double qValue, G4double excitedEnergy) {
  fEnergy = energy;
  fCMEnergy = cmEnergy;
  fVertexZ = vertexZ;
  fQValue = qValue;
  fExcitedEnergy = excitedEnergy;
}

inline void MMRunData::AddLightRecoil(G4double angleCM, G4double angleLab, G4double energy,
                                      G4int charge, G4int mass) {
  fLightAngleCM = angleCM;
  fLightAngleLab = angleLab;
  fLightEnergy = energy;
  fLightRecoilCharge = charge;
  fLightRecoilMass = mass;
}

inline void MMRunData::AddHeavyRecoil(G4double angleCM, G4double angleLab, G4double energy,
                                      G4int charge, G4int mass) {
  fHeavyAngleCM = angleCM;
  fHeavyAngleLab = angleLab;
  fHeavyEnergy = energy;
  fHeavyRecoilCharge = charge;
  fHeavyRecoilMass = mass;
}

#endif