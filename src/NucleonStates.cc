#include "NucleonStates.hh"

NucleonStates* NucleonStates::fInstance = NULL;

NucleonStates::NucleonStates() {
  GenerateStates();
}

NucleonStates* NucleonStates::Instance() {
  if(!fInstance) {
    fInstance = new NucleonStates();
  }
  return fInstance;
}

void NucleonStates::GenerateStates() {
  B8();
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

}

void NucleonStates::Li7() {

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