#include "NucleonStates.hh"

NucleonStates* NucleonStates::fInstance = NULL;

NucleonStates::NucleonStates() {
  GenerateStates();
  MakeCDF();
}

NucleonStates* NucleonStates::Instance() {
  if(!fInstance) {
    fInstance = new NucleonStates();
  }
  return fInstance;
}

// These are the nuclei and thresholds that can be excited and decayed from
void NucleonStates::GenerateStates() {
  He4();
  Li6();
  Li7();
  Be7();
  Be8();
  B8();
}

void NucleonStates::MakeCDF() {
  for(auto &ent1 : nucleonStates) {
    for(auto &ent2 : ent1.second) {
      G4double totalProbability = 0.;
      std::vector<excitedStruct> &states = ent2.second;
      for(auto &state : states) {
        totalProbability += state.probability;
      }
      for(size_t i = 0; i < states.size(); i++) {
        states[i].probability /= totalProbability;
        if(i > 0) states[i].probability = states[i - 1].probability + states[i].probability;
      }
    }
  }
}

G4double NucleonStates::GetExcitedLevel(G4int charge, G4int mass, G4double probability) {
  std::vector<excitedStruct> states = nucleonStates[charge][mass];
  G4double exEnergy = 0.;
  size_t energyLoc = 0;
  if(probability < states[0].probability) energyLoc = 0;
  else {
    G4double minDistance = 1000.;
    for(size_t i = 0; i < states.size() - 1; i++) {
      if(probability > states[i].probability && (probability - states[i].probability) < minDistance) {
        minDistance = probability - states[i].probability;
        energyLoc = i + 1;
      }
    }
  }
  return states[energyLoc].energy;
}

std::vector<thresholdStruct> NucleonStates::GetThresholds(G4int charge, G4int mass) {
  return nucleonThresholds[charge][mass];
}

void NucleonStates::He4() {
  std::vector<excitedStruct> states;

  excitedStruct state = {0., 1.};
  states.push_back(state);

  nucleonStates[2][4] = states;
}

void NucleonStates::Li6() {
  std::vector<excitedStruct> states;

  excitedStruct state = {0., 0.95};
  states.push_back(state);

  state = {2.186, 0.05};
  states.push_back(state);

  std::vector<thresholdStruct> thresholds;

  thresholdStruct threshold = {1.4743, 1, 2, 2, 4};
  thresholds.push_back(threshold);

  nucleonStates[3][6] = states;
  nucleonThresholds[3][6] = thresholds;
}

void NucleonStates::Li7() {
  std::vector<excitedStruct> states;

  excitedStruct state = {0., 0.8};
  states.push_back(state);

  state = {0.47761, 0.2};
  states.push_back(state);

  nucleonStates[3][7] = states;
}

void NucleonStates::Be7() {
  std::vector<excitedStruct> states;

  excitedStruct state = {0., 0.8};
  states.push_back(state);

  state = {0.4291, 0.2};
  states.push_back(state);

  nucleonStates[4][7] = states;
}

void NucleonStates::Be8() {
  std::vector<excitedStruct> states;

  excitedStruct state = {0., 1.};
  states.push_back(state);

  std::vector<thresholdStruct> thresholds;

  thresholdStruct threshold = {-0.0918, 2, 4, 2, 4};
  thresholds.push_back(threshold);

  nucleonStates[4][8] = states;
  nucleonThresholds[4][8] = thresholds;
}

void NucleonStates::B8() {
  std::vector<excitedStruct> states;

  excitedStruct state = {0., 0.6};
  states.push_back(state);

  state = {0.7695, 0.3};
  states.push_back(state);

  state = {2.32, 0.1};
  states.push_back(state);

  std::vector<thresholdStruct> thresholds;

  thresholdStruct threshold = {0.1375, 1, 1, 4, 7};
  thresholds.push_back(threshold);

  nucleonStates[5][8] = states;
  nucleonThresholds[5][8] = thresholds;
}