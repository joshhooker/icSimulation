#ifndef MMEventAction_h
#define MMEventAction_h

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
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
#include <TRandom3.h>

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#include <assert.h>
#include <cstdio>
#include <map>
#include <vector>

class MMEventAction : public G4UserEventAction {

public:
  MMEventAction(G4int numGrids);
  ~MMEventAction();

  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

  void SetFanoFactor(G4double fanoFactor) {fFanoFactor = fanoFactor;}
  void SetWorkFunction(G4double workFunction) {fWorkFunction = workFunction;}
  void SetScintillatorResolution(G4double resolution) {fScintResolution = resolution;}
  void SetGridResolution(G4double resolution) {fGridResolution = resolution;}
  void SetNumGrids(G4int numGrids) {fNumGrids = numGrids;}

private:
  std::vector<G4int> fICGridHCID;
  G4int fScintHCID;

  std::vector<G4double> fICGridE;

  G4double fFanoFactor;
  G4double fWorkFunction;
  G4double fScintResolution;
  G4double fGridResolution;
  G4int fNumGrids;

  gsl_rng *r;
  TRandom3 *fRandom3;
};

#endif
