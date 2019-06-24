#include "NucleonStates.hh"

NucleonStates* NucleonStates::fInstance = NULL;

NucleonStates::NucleonStates() {
    ReadJSON();
}

NucleonStates* NucleonStates::Instance() {
    if(!fInstance) {
        fInstance = new NucleonStates();
    }
    return fInstance;
}

void NucleonStates::ReadJSON() {
    // Read and parse nuclear_states.json
    Json::Value config_nuclear_states;
    std::ifstream config_nuclear_states_stream("nuclear_states.json");
    ASSERT_WITH_MESSAGE(config_nuclear_states_stream.is_open(), 
        "Could not find 'nuclear_states.json'\n");
    config_nuclear_states_stream >> config_nuclear_states;
    config_nuclear_states_stream.close();

    std::vector<isotope_struct> isotopes;
    for(int i = 0; i < config_nuclear_states["Isotopes"].size(); i++) {
        std::string name = config_nuclear_states["Isotopes"][i]["Name"].asString();
        uint charge = config_nuclear_states["Isotopes"][i]["ZA"][0].asUInt();
        uint mass = config_nuclear_states["Isotopes"][i]["ZA"][1].asUInt();

        std::vector<state_struct> states;
        for(int j = 0; j < config_nuclear_states["Isotopes"][i]["States"].size(); j++) {
            double energy = config_nuclear_states["Isotopes"][i]["States"][j]["Energy"].asDouble();
            uint spin;
            std::string spinparity = config_nuclear_states["Isotopes"][i]["States"][j]["Spin-parity"].asString();
            char parity_char = spinparity.back();
            bool parity, good_state;
            if(parity_char == '+') {
                parity = true;
                good_state = true;
            }
            else if(parity_char == '-') {
                parity = false;
                good_state = true;
            }
            else {
                printf("Spin-parity for isotope %s with energy %f not set correctly!\n", name.c_str(), energy);
                printf("Setting state to 0+\n");
                parity = true;
                good_state = false;
            }
            if(good_state) {
                std::string spinparity_copy = spinparity;
                spinparity_copy.pop_back();
                double spind = stof(spinparity_copy)*2;
                spin = static_cast<int>(spind);
            }
            else {
                spin = 0;
            }
            double probability = 1.;
            if(config_nuclear_states["Isotopes"][i]["States"][j]["Probability"].isDouble()) {
                probability = config_nuclear_states["Isotopes"][i]["States"][j]["Probability"].asDouble();
            }
            state_struct state = {energy, spin, parity, probability};
            states.push_back(state);
        }

        double totalProbability = 0.;
        for(auto state: states) {
            totalProbability += state.probability;
        }
        for(size_t i = 0; i < states.size(); i++) {
            states[i].probability /= totalProbability;
            if(i > 0) states[i].probability = states[i - 1].probability + states[i].probability;
        }

        std::vector<theshold_struct> thresholds;
        for(int j = 0; j < config_nuclear_states["Isotopes"][i]["Thresholds"].size(); j++) {
            double energy = config_nuclear_states["Isotopes"][i]["Thresholds"][j]["Energy"].asDouble();
            uint threshold_charge = config_nuclear_states["Isotopes"][i]["Thresholds"][j]["Decay"][0].asUInt();
            uint threshold_mass = config_nuclear_states["Isotopes"][i]["Thresholds"][j]["Decay"][1].asUInt();
            theshold_struct threshold = {energy, threshold_charge, threshold_mass};
            thresholds.push_back(threshold);
        }

        isotope_struct isotope = {name, charge, mass, states, thresholds};
        isotopes.push_back(isotope);
    }

    printf("Printing nuclear table to use:\n");
    for(auto isotope: isotopes) {
        printf("Name: %5s; Z: %3d; A: %3d\n", isotope.name.c_str(), isotope.charge, isotope.mass);
        printf("\t States:\n");
        for(auto state: isotope.states) {
            printf("\t\tEnergy: %8.3f; Spin*2: %2d; Parity: %d; Probability: %f\n", state.energy, state.spin2, state.parity, state.probability);
        }
        printf("\t Thresholds:\n");
        for(auto threshold: isotope.thresholds) {
            printf("\t\tEnergy: %8.3f; Decay Product [%3d, %3d]\n", threshold.energy, threshold.decay_charge, threshold.decay_mass);
        }
        nucleons_[isotope.charge][isotope.mass] = isotope;
    }
}

G4double NucleonStates::GetExcitedLevel(uint charge, uint mass, G4double probability) {
  std::vector<state_struct> states = nucleons_[charge][mass].states;
  if(states.empty()) {
      return 0.;
  }
  G4double exEnergy = 0.;
  if(probability < states[0].probability) exEnergy = states[0].energy;
  else {
    for(size_t i = 1; i < states.size(); i++) {
      if(probability > states[i - 1].probability && probability < states[i].probability) {
        exEnergy = states[i].energy;
      }
    }
  }
  return exEnergy;
}

std::vector<theshold_struct> NucleonStates::GetThresholds(G4int charge, G4int mass) {
  return nucleons_[charge][mass].thresholds;
}