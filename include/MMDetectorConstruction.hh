#ifndef MMDetectorConstruction_h
#define MMDetectorConstruction_h

#include "G4Box.hh"
#include "G4ChordFinder.hh"
#include "G4ClassicalRK4.hh"
#include "G4Colour.hh"
#include "G4EqMagElectricField.hh"
#include "G4ExtrudedSolid.hh"
#include "G4FieldManager.hh"
#include "G4GenericMessenger.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4Tubs.hh"
#include "G4TwoVector.hh"
#include "G4UniformElectricField.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"

#include "MMGenSD.hh"

#include <algorithm>
#include <iostream>
#include <string>

class MMDetectorConstruction : public G4VUserDetectorConstruction {

public:
  MMDetectorConstruction();
  virtual ~MMDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  void SetGasPressure(G4double pressure) {
    fPressureInTorr = pressure;
    G4cout << "SET: Gas Pressure (torr): " << fPressureInTorr << G4endl;
  }

  void SetGasTemperature(G4double temperature) {
    fTemperature = temperature;
    G4cout << "SET: Gas Temperature (K): " << fTemperature << G4endl;
  }

  void SetGasType(G4String type) {
    fGasType = type;
    G4cout << "SET: Gas Type: " << fGasType << G4endl;
  }

  void SetNumGrids(G4int numGrids) {
    fNumGrids = numGrids;
    G4cout << "SET: Number of IC Grids: " << fNumGrids << G4endl;
  }

  void SetGridSize(G4double size) {
    fGridSize = size;
    if(fUseInches) {
      G4cout << "SET: Grid Distance (in): " << fGridSize << G4endl;
    }
    else {
      G4cout << "SET: Grid Distance (mm): " << fGridSize << G4endl;
    }
  }

  void SetGridRadius(G4double radius) {
    fGridRadius = radius;
    if(fUseInches) {
      G4cout << "SET: Grid Radius (in): " << fGridRadius << G4endl;
    }
    else {
      G4cout << "SET: Grid Radius (mm): " << fGridRadius << G4endl;
    }
  }

  void SetGridDistance(G4double distance) {
    fGridDist = distance;
    if(fUseInches) {
      G4cout << "SET: Grid Distance (in): " << fGridDist << G4endl;
    }
    else {
      G4cout << "SET: Grid Distance (mm): " << fGridDist << G4endl;
    }
  }

  void SetScintDistance(G4double dist) {
    fScintDist = dist;
    if(fUseInches) {
      G4cout << "SET: Distance from window to scintillator (in): " << fScintDist << G4endl;
    }
    else {
      G4cout << "SET: Distance from window to scintillator (mm): " << fScintDist << G4endl;
    }
  }

  void SetWireThickness(G4double wireThickness) {
    fWireThickness = wireThickness;
    if(fUseInches) {
      G4cout << "SET: Wire Thickness (in): " << fWireThickness << G4endl;
    }
    else {
      G4cout << "SET: Wire Thickness (mm): " << fWireThickness << " (" << fWireThickness*1000. << " um)" << G4endl;
    }
  }

  void SetUseInches(G4bool inch) {
    fUseInches = inch;
    if(fUseInches) {
      G4cout << "SET: Using inches from input" << G4endl;
    }
    else {
      G4cout << "SET: Using mm from input" << G4endl;
    }
  }

  G4Material* GetGasMaterial() {return fGasMaterial;}

  G4LogicalVolume* GetWorldVolume() const {return fWorldLogical;}
  G4LogicalVolume* GetTargetVolume() const {return fTargetLogical;}
  G4LogicalVolume* GetDetectVolume() const {return fDetectLogical;}
  G4LogicalVolume* GetScintVolume() const {return fScintLogical;}
  G4LogicalVolume* GetFoilVolume() const {return fFoilLogical;}
  G4LogicalVolume* GetGridVolume(G4int grid) const {return fGridLogical[grid];}
  G4LogicalVolume* GetWireVolume(G4int grid, G4int wire) const {return fWireGridLogical[wire][grid];}

private:
  G4double fPressureInTorr;
  G4double fTemperature;
  G4String fGasType;
  G4Material* fGasMaterial;

  G4double inchtocm;
  G4int fNumGrids;
  G4double fGridSize;
  G4double fGridRadius;
  G4double fGridDist;
  G4double fScintDist;
  G4double fWireThickness;
  G4bool fUseInches;

  G4LogicalVolume* fWorldLogical;
  G4LogicalVolume* fTargetLogical;
  G4LogicalVolume* fDetectLogical;
  G4LogicalVolume* fFoilLogical;
  std::vector<G4LogicalVolume*> fGridLogical;
  std::vector<G4LogicalVolume*> fWireGridLogical[49];
  G4LogicalVolume* fScintLogical;

  void ConstructMaterials();
  void SetAttributes();
};

#endif
