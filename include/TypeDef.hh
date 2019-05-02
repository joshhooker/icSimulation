#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef struct excitedStruct {
  G4double energy;
  G4double probability;
} excitedState;

typedef struct thresholdStruct {
  G4double thresholdEnergy;
  G4int lightDecayCharge;
  G4int lightDecayMass;
  G4int heavyDecayCharge;
  G4int heavyDecayMass;
} thresholdState;

#endif //TYPEDEF_H