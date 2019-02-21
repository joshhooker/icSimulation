#ifndef MMEventAction_h
#define MMEventAction_h

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4UserEventAction.hh"

#include "MMAnalysis.hh"
#include "MMGenHit.hh"
#include "Randomize.hh"

#include "TROOT.h"
#include <TH1F.h>
#include <TH2F.h>

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#include <map>
#include <assert.h>
#include <cstdio>

class MMEventAction : public G4UserEventAction {

public:
  MMEventAction();
  ~MMEventAction();

  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
  void ParseParams(const std::map<std::string, G4double>&);

private:
  G4int fICGridHCID[4];
  G4int fScintHCID;

  G4double fScintE;
  G4double fICGrid1E, fICGrid2E, fICGrid3E, fICGrid4E;

  G4double fFanoFactor;
  G4double fWorkFunction;

  gsl_rng * r;
};

#endif
