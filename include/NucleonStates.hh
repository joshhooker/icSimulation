#ifndef NUCLEONSTATES_H
#define NUCLEONSTATES_H

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "G4Types.hh"
#include "globals.hh"

#include "json/json.h"
#include "TypeDef.hh"

class NucleonStates {
public:
  NucleonStates();
  static NucleonStates* Instance();

  void ReadJSON();
  G4double GetExcitedLevel(uint, uint, G4double);
  std::vector<theshold_struct> GetThresholds(G4int, G4int);

private:
  static NucleonStates* fInstance;

  std::map<uint, std::map<uint, isotope_struct> > nucleons_;
};

#endif