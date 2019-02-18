#ifndef MMRunAction_h
#define MMRunAction_h

#include "G4UserRunAction.hh"

#include "MMAnalysis.hh"

class MMRunAction : public G4UserRunAction {
public:
  MMRunAction();
  ~MMRunAction();

  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
};

#endif
