#ifndef MMEventAction_h
#define MMEventAction_h

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4UserEventAction.hh"

#include "MMGenHit.hh"
#include "MMRunData.hh"

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

    void SetEnergy(G4double energy) {fEnergy = energy;}
    void SetCMEnergy(G4double cmEnergy) {fCMEnergy = cmEnergy;}
    void SetVertexZ(G4double vertexZ) {fVertexZ = vertexZ;}
    void SetQValue(G4double qValue) {fQValue = qValue;}
    void SetExcitedEnergy(G4double excitedEnergy) {fExcitedEnergy = excitedEnergy;}

    void SetLightAngleCM(G4double angle) {fLightAngleCM = angle;}
    void SetLightAngleLab(G4double angle) {fLightAngleLab = angle;}
    void SetLightEnergy(G4double energy) {fLightEnergy = energy;}

    void SetHeavyAngleCM(G4double angle) {fHeavyAngleCM = angle;}
    void SetHeavyAngleLab(G4double angle) {fHeavyAngleLab = angle;}
    void SetHeavyEnergy(G4double energy) {fHeavyEnergy = energy;}

    void SetLightRecoilCharge(G4int charge) {fLightRecoilCharge = charge;}
    void SetLightRecoilMass(G4int mass) {fLightRecoilMass = mass;}
    void SetHeavyRecoilCharge(G4int charge) {fHeavyRecoilCharge = charge;}
    void SetHeavyRecoilMass(G4int mass) {fHeavyRecoilMass = mass;}

    std::vector<G4double>& GetGridEnergy() {return fGridEnergy;}
    std::vector<G4int>& GetGridID() {return fGridID;}

    std::vector<G4double>& GetScintXPosition() {return fScintXPosition;}
    std::vector<G4double>& GetScintYPosition() {return fScintYPosition;}
    std::vector<G4double>& GetScintEnergy() {return fScintEnergy;}
    std::vector<G4int>& GetScintTrackID() {return fScintTrackID;}
    std::vector<G4int>& GetScintMass() {return fScintMass;}
    std::vector<G4int>& GetScintCharge() {return fScintCharge;}

private:
    std::vector<G4int> fICGridHCID;
    G4int fScintHCID;

    std::vector<G4double> fICGridE;

    G4double fFanoFactor;
    G4double fWorkFunction;
    G4double fScintResolution;
    G4double fGridResolution;
    G4int fNumGrids;

    std::vector<G4double> fGridEnergy;
    std::vector<G4int> fGridID;

    std::vector<G4double> fScintXPosition;
    std::vector<G4double> fScintYPosition;
    std::vector<G4double> fScintEnergy;
    std::vector<G4int> fScintTrackID;
    std::vector<G4int> fScintMass;
    std::vector<G4int> fScintCharge;

    G4double fBeamEnergy;
    G4int fBeamCharge;
    G4int fBeamMass;

    G4double fEnergy;
    G4double fCMEnergy;
    G4double fVertexZ;
    G4double fQValue;
    G4double fExcitedEnergy;

    G4double fLightAngleCM;
    G4double fLightAngleLab;
    G4double fLightEnergy;
    G4double fHeavyAngleCM;
    G4double fHeavyAngleLab;
    G4double fHeavyEnergy;

    G4int fLightRecoilCharge;
    G4int fLightRecoilMass;
    G4int fHeavyRecoilCharge;
    G4int fHeavyRecoilMass;

    gsl_rng *r;
    TRandom3 *fRandom3;
};

#endif
