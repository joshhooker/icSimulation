#include "MMDetectorConstruction.hh"

MMDetectorConstruction::MMDetectorConstruction() :
  G4VUserDetectorConstruction(),
  fPressureInTorr(760.),
  fTemperature(293.15) {
}

MMDetectorConstruction::~MMDetectorConstruction() {
}

G4VPhysicalVolume* MMDetectorConstruction::Construct() {
  ConstructMaterials();

  inchtocm = 2.54;

  // Define Elements
  G4Element* H = new G4Element("Hydrogen", "H", 1, 1.008*g/mole);
  G4Element* C = new G4Element("Carbon", "C", 6, 12.011*g/mole);
  G4Element* O = new G4Element("Oxygen", "O", 8, 15.999*g/mole);
  G4Element* Ar = new G4Element("Argon", "Ar", 18, 39.948*g/mole);
  G4Element* Cr = new G4Element("Chrome", "Cr", 25, 51.996*g/mole);
  G4Element* Fe = new G4Element("Iron", "Fe", 26, 55.845*g/mole);
  G4Element* Co = new G4Element("Cobalt", "Co", 27, 58.933*g/mole);
  G4Element* Ni = new G4Element("Nickel", "Ni", 28, 58.693*g/mole);
  G4Element* W  = new G4Element("Tungsten", "W", 74, 183.850*g/mole);

  // Define Havar
  G4Material* Havar = new G4Material("Havar", 8.3*g/cm3, 5);
  Havar->AddElement(Cr, 0.1785);
  Havar->AddElement(Fe, 0.1822);
  Havar->AddElement(Co, 0.4452);
  Havar->AddElement(Ni, 0.1310);
  Havar->AddElement(W, 0.0631);

  // Define Mylar
  G4Material* Mylar = new G4Material("Mylar", 1.397*g/cm3, 3);
  Mylar->AddElement(H, 8);
  Mylar->AddElement(C, 10);
  Mylar->AddElement(O, 4);

  // Define BC-400 Plastic Scintillator
  G4Material* BC400 = new G4Material("BC400", 1.032*g/cm3, 2);
  BC400->AddElement(H, 10);
  BC400->AddElement(C, 9);

  // Define P10
  // P10 at  5 torr (room temp): 0.0009921 g/cm3
  // P10 at 10 torr (room temp): 0.0019843 g/cm3
  // P10 at 20 torr (room temp): 0.0039685 g/cm3
  // P10 at 30 torr (room temp): 0.0059528 g/cm3
  // P10 at 40 torr (room temp): 0.0079371 g/cm3
  G4Material* P10 = new G4Material("P10", 0.0009921*g/cm3, 3);
  P10->AddElement(H, 0.0155);
  P10->AddElement(C, 0.0623);
  P10->AddElement(Ar, 0.9222);

  // Define C2D4 (Deurated Polyethylene)
  G4Isotope* iso_H2 = new G4Isotope("H2", 1, 2, 2.014*g/mole);
  G4Element* D = new G4Element("Deuterium Atom", "D", 1);
  D->AddIsotope(iso_H2, 1.);

  G4Material* C2D4 = new G4Material("C2D4", 1.06*g/cm3, 2);
  C2D4->AddElement(C, 2);
  C2D4->AddElement(D, 4);

  G4Material* vacuum =
    new G4Material("Vacuum",      //Name as String
		   1,             //Atomic Number,  in this case we use 1 for hydrogen
		   1.008*g/mole,  //Mass per Mole "Atomic Weight"  1.008*g/mole for Hydoren
		   1.e-25*g/cm3,  //Density of Vaccuum  *Cant be Zero, Must be small instead
		   kStateGas,     //kStateGas for Gas
		   2.73*kelvin,   //Temperatuer for ga
		   1.e-25*g/cm3); //Pressure for Vaccum

  // Overlaps flag
  G4bool checkOverlaps = false;

  // Create vacuum filled world
  G4VSolid* worldSolid = new G4Box("worldBox", 0.2*m, 0.15*m, 0.7*m);
  fWorldLogical = new G4LogicalVolume(worldSolid, vacuum, "worldLogical");
  G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, G4ThreeVector(), fWorldLogical, "worldPhysical", 0,
                                                       false, 0, checkOverlaps);

  // C2D4 Target
  // 6.60 um for 0.7 mg/cm2
  // 3.77 um for 0.4 mg/cm2
  G4VSolid* targetSolid = new G4Tubs("targetSolid", 0., 10*mm, 6.60/2.*um, 0., 360.*deg);
  fTargetLogical =  new G4LogicalVolume(targetSolid, C2D4, "targetLogical");
  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), fTargetLogical, "targetPhysical", fWorldLogical,
                                                        false, 0, checkOverlaps);

  // P10 chamber
  G4VSolid* detectSolid = new G4Tubs("detectBox", 0., 0.1*m, 0.3/2.*m, 0., 360.*deg);
  fDetectLogical = new G4LogicalVolume(detectSolid, P10, "detectLogical");
  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.367*m), fDetectLogical, "detectPhysical", fWorldLogical,
                    false, 0, checkOverlaps);

  // Mylar foil
  G4VSolid* foilSolid = new G4Tubs("foilSolid", 0., 2.75*inchtocm*cm, 4./2.*um, 0., 360.*deg);
  fFoilLogical = new G4LogicalVolume(foilSolid, Mylar, "foilLogical");
  new G4PVPlacement(0, G4ThreeVector(0., 0., -143.7*mm), fFoilLogical, "foilPhysical", fDetectLogical,
                    false, 0, checkOverlaps);

  // Current configuration: 4 grids of 0.5 inches each in the middle of the foil and scintillator (scintillator is 14.14 inches from the target or 5.6 inches from the foil)
  // Assuming the scintillator has a 2.75 in radius and recoil cone of 11 degrees

  G4double foilToScintillator = 5.6*inchtocm*cm;
  G4double scintillatorDetectPos = -0.7*mm;

  // IC grids
  char name[256];
  G4double distancePerGrid = 0.2*inchtocm*cm; // 0.5 inches
  numGrids = 4;
  G4double distanceTotalGrid = distancePerGrid*static_cast<G4double>(numGrids);
  G4double midGrid = foilToScintillator/2.; // Picking halfway between foil and scintillator
  for(int i = 0; i < numGrids; i++) {
    sprintf(name, "grid%d", i + 1);
    G4VSolid* gridSolid = new G4Tubs(name, 0., 2.75*inchtocm*cm, distancePerGrid/2., 0., 360.*deg);
    sprintf(name, "gridLogical%d", i + 1);
    fGridLogical[i] = new G4LogicalVolume(gridSolid, P10, name);
    sprintf(name, "gridPhysical%d", i + 1);
    new G4PVPlacement(0, G4ThreeVector(0., 0., scintillatorDetectPos - midGrid - distanceTotalGrid/2. + distancePerGrid/2. + (i - 1)*distancePerGrid), fGridLogical[i], name, fDetectLogical,
                      false, 0, checkOverlaps);
  }

  // BC-400 Plastic Scintillator
  G4VSolid* scintSolid = new G4Tubs("scintSolid", 0., 2.75*inchtocm*cm, 1.0*inchtocm*cm/2., 0., 360.*deg);
  fScintLogical =  new G4LogicalVolume(scintSolid, BC400, "scintLogical");
  new G4PVPlacement(0, G4ThreeVector(0., 0., scintillatorDetectPos), fScintLogical, "scintPhysical", fDetectLogical,
                    false, 0, checkOverlaps);

  SetAttributes();

  return worldPhysical;
}

void MMDetectorConstruction::ConstructMaterials() {
  G4NistManager* man = G4NistManager::Instance();
}

void MMDetectorConstruction::ConstructSDandField() {
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String SDname;

  char name[256];

  // IC Grids
  for(int i = 0; i < numGrids; i++) {
    char nameGrid[256];
    sprintf(nameGrid,"grid%d", i + 1);
    G4VSensitiveDetector* gridDetector = new MMGenSD(SDname = nameGrid);
    SDman->AddNewDetector(gridDetector);
    fGridLogical[i]->SetSensitiveDetector(gridDetector);
  }

  // BC-400 Plastic Scintillator
  char nameScint[] = "scint";
  G4VSensitiveDetector* scintDetector = new MMGenSD(SDname = nameScint);
  SDman->AddNewDetector(scintDetector);
  fScintLogical->SetSensitiveDetector(scintDetector);
}

void MMDetectorConstruction::SetAttributes() {
  G4VisAttributes* worldAttr = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  worldAttr->SetVisibility(true);
  worldAttr->SetForceWireframe(true);
  fWorldLogical->SetVisAttributes(worldAttr);

  G4VisAttributes* detectAttr = new G4VisAttributes(G4Colour(0., 0., 1.0));
  detectAttr->SetVisibility(true);
  detectAttr->SetForceWireframe(true);
  fDetectLogical->SetVisAttributes(detectAttr);

  G4VisAttributes* targetAttr = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  targetAttr->SetVisibility(true);
  targetAttr->SetForceSolid(true);
  fTargetLogical->SetVisAttributes(targetAttr);

  G4VisAttributes* foilAttr = new G4VisAttributes(G4Colour::White());
  foilAttr->SetVisibility(true);
  foilAttr->SetForceSolid(true);
  fFoilLogical->SetVisAttributes(foilAttr);

  G4Colour gridColors[10] = {G4Colour::Yellow(), G4Colour::Magenta(), G4Colour::Cyan(), G4Colour::Green(), G4Colour::White(),
                             G4Colour::Yellow(), G4Colour::Magenta(), G4Colour::Cyan(), G4Colour::Green(), G4Colour::White()};

  for(int i = 0; i < numGrids; i++) {
    G4VisAttributes* gridAttr = new G4VisAttributes(gridColors[i]);
    gridAttr->SetVisibility(true);
    gridAttr->SetForceSolid(true);
    fGridLogical[i]->SetVisAttributes(gridAttr);
  }

  G4VisAttributes* scintAttr = new G4VisAttributes(G4Colour::Red());
  scintAttr->SetVisibility(true);
  scintAttr->SetForceSolid(true);
  fScintLogical->SetVisAttributes(scintAttr);
}
