#ifndef NUCLEONSTATES_H
#define NUCLEONSTATES_H

#include "G4Types.hh"

#include "TypeDef.hh"

#include <map>
#include <vector>

class NucleonStates {
public:
  NucleonStates();
  static NucleonStates* Instance();

  void GenerateStates();
  std::vector<excitedStruct> GetExcitedLevel(G4int charge, G4int mass);
  std::vector<thresholdStruct> GetThresholds(G4int charge, G4int mass);

private:
  static NucleonStates* fInstance;

  void He4();
  void Li6();
  void Li7();
  void Be7();
  void Be8();
  void B8();

  std::map<int, std::map<int, std::vector<excitedStruct> > > nucleonStates;
  std::map<int, std::map<int, std::vector<thresholdStruct> > > nucleonThresholds;
};

#endif