#ifndef MMPrimaryGeneratorAction_h
#define MMPrimaryGeneratorAction_h

#include "G4GenericMessenger.hh"
#include "G4IonTable.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "Randomize.hh"

#include "MMAnalysis.hh"
#include "MMEventAction.hh"

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#include <ctime>

class G4ParticleGun;
class G4ParticleDefinition;
class G4GenericMessenger;

class MMPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

public:
  MMPrimaryGeneratorAction();
  virtual ~MMPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

  G4ParticleGun* GetParticleGun() {return fParticleGun;};

private:
  G4ParticleGun* fParticleGun;
  G4ParticleDefinition* fParticle;
  G4GenericMessenger* fMessenger;

  void DefineCommands();

  const gsl_rng_type *T;
  gsl_rng *r;
};

#endif
