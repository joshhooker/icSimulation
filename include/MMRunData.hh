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
  virtual ~MMRunData();

private:
};

#endif