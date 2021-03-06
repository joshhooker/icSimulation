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

  void SetFanoFactor(G4double fanoFactor) {fFanoFactor = fanoFactor;}
  void SetWorkFunction(G4double workFunction) {fWorkFunction = workFunction;}
  void SetNumGrids(G4int numGrids) {fNumGrids = numGrids;}
  void SetScintillatorResolution(G4double resolution) {fScintResolution = resolution;}
  void SetGridResolution(G4double resolution) {fGridResolution = resolution;}

private:
  MMDetectorConstruction* fDetector;
  std::map<std::string, G4double> fEventActionParams;
  G4double fFanoFactor;
  G4double fWorkFunction;
  G4int fNumGrids;
  G4double fScintResolution;
  G4double fGridResolution;
};

#endif
