#ifndef MMActionInitialization_h
#define MMActionInitialization_h

#include "G4VUserActionInitialization.hh"

#include "MMPrimaryGeneratorAction.hh"
#include "MMEventAction.hh"
#include "MMRunAction.hh"
#include "MMTrackingAction.hh"

#include <cstdio>

class MMActionInitialization : public G4VUserActionInitialization {
public:
  MMActionInitialization();
  virtual ~MMActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

private:

};

#endif
