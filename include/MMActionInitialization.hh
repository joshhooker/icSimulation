#ifndef MMActionInitialization_h
#define MMActionInitialization_h

#include "G4VUserActionInitialization.hh"

#include "MMDetectorConstruction.hh"
#include "MMEventAction.hh"
#include "MMPrimaryGeneratorAction.hh"
#include "MMRunAction.hh"
#include "MMTrackingAction.hh"

#include <cstdio>
#include <map>

class MMDetectorConstruction;
class MMEventAction;

class MMActionInitialization : public G4VUserActionInitialization {
public:
  MMActionInitialization(MMDetectorConstruction*);
  virtual ~MMActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

  void SetEventActionParams(std::map<std::string, G4double> params) {
    fEventActionParams = params;
  };

private:
    MMDetectorConstruction* fDetector;
    std::map<std::string, G4double> fEventActionParams;
};

#endif
