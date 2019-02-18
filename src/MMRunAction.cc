#include "MMRunAction.hh"

MMRunAction::MMRunAction() :
  G4UserRunAction() {
  MMAnalysis::Instance();
}

MMRunAction::~MMRunAction() {
  delete MMAnalysis::Instance();
}

void MMRunAction::BeginOfRunAction(const G4Run*) {
  MMAnalysis::Instance()->OpenFile();
}

void MMRunAction::EndOfRunAction(const G4Run*) {
  MMAnalysis::Instance()->CloseFile();
}
