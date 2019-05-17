#include "MMDetectorConstruction.hh"

MMDetectorConstruction::MMDetectorConstruction() :
  G4VUserDetectorConstruction()
{}

MMDetectorConstruction::~MMDetectorConstruction() {}

G4VPhysicalVolume* MMDetectorConstruction::Construct() {
  ConstructMaterials();

  // Overlaps flag
  G4bool checkOverlaps = false;

  G4double z, fractionmass;
  G4int nel, natoms;

  inchtocm = 2.54;

  // Define Elements
  G4Element* H  = new G4Element("Hydrogen", "H",  z = 1.,  1.008*g/mole);
  G4Element* C  = new G4Element("Carbon",   "C",  z = 6.,  12.011*g/mole);
  G4Element* O  = new G4Element("Oxygen",   "O",  z = 8.,  15.999*g/mole);
  G4Element* F  = new G4Element("Fluorine", "F",  z = 9.,  18.998*g/mole);
  G4Element* Ar = new G4Element("Argon",    "Ar", z = 18., 39.948*g/mole);
  G4Element* Cr = new G4Element("Chrome",   "Cr", z = 25., 51.996*g/mole);
  G4Element* Fe = new G4Element("Iron",     "Fe", z = 26., 55.845*g/mole);
  G4Element* Co = new G4Element("Cobalt",   "Co", z = 27., 58.933*g/mole);
  G4Element* Ni = new G4Element("Nickel",   "Ni", z = 28., 58.693*g/mole);
  G4Element* W  = new G4Element("Tungsten", "W",  z = 74., 183.850*g/mole);
  G4Element* Au = new G4Element("Gold",     "Au", z = 79., 196.967*g/mole);

  // Define Havar
  G4Material* Havar = new G4Material("Havar", 8.3*g/cm3, nel = 5);
  Havar->AddElement(Cr, fractionmass = 0.1785);
  Havar->AddElement(Fe, fractionmass = 0.1822);
  Havar->AddElement(Co, fractionmass = 0.4452);
  Havar->AddElement(Ni, fractionmass = 0.1310);
  Havar->AddElement(W,  fractionmass = 0.0631);

  // Define Mylar
  G4Material* Mylar = new G4Material("Mylar", 1.397*g/cm3, nel = 3);
  Mylar->AddElement(H, natoms = 8);
  Mylar->AddElement(C, natoms = 10);
  Mylar->AddElement(O, natoms = 4);

  // Define BC-400 Plastic Scintillator
  G4Material* BC400 = new G4Material("BC400", 1.032*g/cm3, nel = 2);
  BC400->AddElement(H, natoms = 10);
  BC400->AddElement(C, natoms = 9);

  // Define C2D4 (Deurated Polyethylene)
  G4Isotope* iso_H2 = new G4Isotope("H2", 1, 2, 2.014*g/mole);
  G4Element* D = new G4Element("Deuterium Atom", "D", 1);
  D->AddIsotope(iso_H2, 1.);

  G4Material* C2D4 = new G4Material("C2D4", 1.06*g/cm3, 2);
  C2D4->AddElement(C, 2);
  C2D4->AddElement(D, 4);

  // Define Tungsten Wires
  G4Material* Tungsten = new G4Material("Tungsten", 19.3*g/cm3, nel = 1);
  Tungsten->AddElement(W, natoms = 1);

  // Define Gold Wires
  G4Material* Gold = new G4Material("Gold", 193*g/cm3, nel = 1);
  Gold->AddElement(Au, natoms = 1);

  G4double atmPressure = 760; // torr

  if(fGasType == "P10" || fGasType == "p10") {
    G4double p10Density = 0.00159*g/cm3;
    fGasMaterial = new G4Material("P10", p10Density*fPressureInTorr/atmPressure, nel = 3, kStateGas,
                                  fTemperature*kelvin, fPressureInTorr*1.333e-3*bar);
    fGasMaterial->AddElement(H,  fractionmass = 0.0155);
    fGasMaterial->AddElement(C,  fractionmass = 0.0623);
    fGasMaterial->AddElement(Ar, fractionmass = 0.9222);
  }
  else if(fGasType == "CO2" || fGasType == "co2") {
    fGasMaterial = G4Material::GetMaterial("CO2");
  }
  else if(fGasType == "Methane" || fGasType == "methane" || fGasType == "METHANE" || fGasType == "CH4") {
    fGasMaterial = G4Material::GetMaterial("Methane");
  }
  else if(fGasType == "CF4" || fGasType == "cf4") {
    G4double cf4Density = 0.0036586*g/cm3;
    fGasMaterial = new G4Material("CF4", cf4Density*fPressureInTorr/atmPressure, nel = 2, kStateGas,
                                  fTemperature*kelvin, fPressureInTorr*1.333e-3*bar);
    fGasMaterial->AddElement(C, natoms = 1);
    fGasMaterial->AddElement(F, natoms = 4);
  }
  else {
    G4cout << "Unknown Gas Type. Either a mistake or you need to add it yourself or ask to add" << G4endl;
    G4cout << "Choosing Methane for you" << G4endl;
    fGasMaterial = G4Material::GetMaterial("Methane");
  }
  G4cout << GetGasMaterial() << G4endl;

  G4Material* vacuum =
    new G4Material("Vacuum",      //Name as String
		   1,             //Atomic Number,  in this case we use 1 for hydrogen
		   1.008*g/mole,  //Mass per Mole "Atomic Weight"  1.008*g/mole for Hydoren
		   1.e-25*g/cm3,  //Density of Vaccuum  *Cant be Zero, Must be small instead
		   kStateGas,     //kStateGas for Gas
       2.73*kelvin,   //Temperatuer for gas
		   1.e-25*g/cm3); //Pressure for Vaccum

  // Create vacuum filled world
  G4VSolid* worldSolid = new G4Box("worldBox", 0.2*m, 0.15*m, 0.7*m);
  fWorldLogical = new G4LogicalVolume(worldSolid, vacuum, "worldLogical");
  G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, G4ThreeVector(), fWorldLogical, "worldPhysical", 0,
                                                       false, 0, checkOverlaps);

  // C2D4 Target
  // 6.60 um for 0.7 mg/cm2
  // 3.77 um for 0.4 mg/cm2
  G4VSolid* targetSolid = new G4Tubs("targetSolid", 0., 20*mm, 6.60/2.*um, 0., 360.*deg);
  fTargetLogical =  new G4LogicalVolume(targetSolid, C2D4, "targetLogical");
  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), fTargetLogical, "targetPhysical", fWorldLogical,
                                                        false, 0, checkOverlaps);

  G4double icChamberLength = 0.3*m;
  G4double targetToWindow = 21.7*cm;

  // IC chamber
  G4VSolid* detectSolid = new G4Tubs("detectBox", 0., 0.1*m, icChamberLength/2., 0., 360.*deg);
  fDetectLogical = new G4LogicalVolume(detectSolid, fGasMaterial, "detectLogical");
  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.367*m), fDetectLogical, "detectPhysical", fWorldLogical,
                    false, 0, checkOverlaps);

  G4double gridDist;
  G4double gridRadius;
  G4double foilRadius;
  G4double scintRadius;
  G4double scintDist;
  G4double wireThickness;

  if(fUseInches) {
    gridDist = fGridDist*inchtocm*cm;
    gridRadius = fGridSize*inchtocm*cm;
    foilRadius = fGridSize*inchtocm*cm;
    scintRadius = fGridSize*inchtocm*cm;
    scintDist = fDistScint*inchtocm*cm;
    wireThickness = fWireThickness*inchtocm*cm;
  }
  else {
    gridDist = fGridDist*mm;
    gridRadius = fGridSize*mm;
    foilRadius = fGridSize*mm;
    scintRadius = fGridSize*mm;
    scintDist = fDistScint*mm;
    wireThickness = fWireThickness*mm;
  }

  // Mylar foil
  G4double foilThickness = 13.*um;
  G4VSolid* foilSolid = foilSolid = new G4Tubs("foilSolid", 0., foilRadius, foilThickness/2., 0., 360.*deg);
  fFoilLogical = new G4LogicalVolume(foilSolid, Mylar, "foilLogical");
  new G4PVPlacement(0, G4ThreeVector(0., 0., targetToWindow - foilThickness/2.), fFoilLogical, "foilPhysical",
                    fWorldLogical, false, 0, checkOverlaps);

  G4double scintillatorDetectPos = -icChamberLength/2. + scintDist;
  G4double scintillatorThickness = 10.*mm;

  // IC grids
  char name[256];
  G4double distancePerGrid = gridDist;
  G4double distanceTotalGrid = distancePerGrid*static_cast<G4double>(fNumGrids);
  G4double midGrid = scintDist/2.; // Picking halfway between foil and scintillator
  G4double midGridPos = -icChamberLength/2. + midGrid; // Position of middle of IC Grids
  // G4cout << distancePerGrid << '\t' << distanceTotalGrid << '\t' << foilToScintillator << '\t' << midGrid << G4endl;
  for(G4int i = 0; i < fNumGrids; i++) {
    sprintf(name, "grid%d", i + 1);
    G4VSolid* gridSolid = new G4Tubs(name, 0., gridRadius, distancePerGrid/2., 0., 360.*deg);
    sprintf(name, "gridLogical%d", i + 1);
    fGridLogical.push_back(new G4LogicalVolume(gridSolid, fGasMaterial, name));
    sprintf(name, "gridPhysical%d", i + 1);
    if(fNumGrids % 2 == 0) {
      int midGridNum = fNumGrids/2;
      if(i < midGridNum) {
        new G4PVPlacement(0, G4ThreeVector(0., 0., midGridPos + distancePerGrid/2. + (i - midGridNum)*distancePerGrid),
                          fGridLogical[i], name, fDetectLogical, false, 0, checkOverlaps);
      }
      else {
        new G4PVPlacement(0, G4ThreeVector(0., 0., midGridPos + distancePerGrid/2. + (i - midGridNum)*distancePerGrid),
                          fGridLogical[i], name, fDetectLogical, false, 0, checkOverlaps);
      }
    }
    else {
      int midGridNum = (fNumGrids - 1)/2 + 1;
      new G4PVPlacement(0, G4ThreeVector(0., 0., midGridPos + (i + 1 - midGridNum)*distancePerGrid), fGridLogical[i],
                        name, fDetectLogical, false, 0, checkOverlaps);
    }
  }

  // IC wires
  G4double wireRadius = wireThickness/2.;
  G4double wireSpacing = 2.794*mm; // 24 wires on each side of center for 49 total
  G4VSolid* gridSolid = new G4Tubs(name, 0., gridRadius, distancePerGrid/2., 0., 360.*deg);
  for(G4int i = 0; i < fNumGrids; i++) {
    sprintf(name, "wire_grid%d", i + 1);
    G4VSolid* wireSolid = new G4Tubs(name, 0., wireRadius, gridRadius*2., 0., 360.*deg);
    G4RotationMatrix *rm = new G4RotationMatrix();
    rm->rotateX(90.*deg);
    for(G4int j = -24; j < 25; j++) {
      sprintf(name, "wire_grid%d_%d", i + 1, j + 1);
      G4VSolid* wireIntersectSolid = new G4IntersectionSolid("Wire-Grid", gridSolid, wireSolid, rm, G4ThreeVector(wireSpacing*j, 0, 0.));
      sprintf(name, "wireGridLogical%d_%d", i + 1, j + 1);
      fWireGridLogical[j + 24].push_back(new G4LogicalVolume(wireIntersectSolid, Gold, name));
    }
    sprintf(name, "wireGridPhysical%d", i + 1);
    for(G4int j = -24; j < 25; j++) {
      G4double frontGrid = -distancePerGrid/2. + wireRadius;
      new G4PVPlacement(0, G4ThreeVector(0., 0, frontGrid), fWireGridLogical[j + 24][i],
                        name, fGridLogical[i], false, j + 24, checkOverlaps);
    }
    for(G4int j = -24; j < 25; j++) {
      new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), fWireGridLogical[j + 24][i],
                        name, fGridLogical[i], false, j + 24 + 49, checkOverlaps);
    }
    if(i == fNumGrids - 1) {
      for(G4int j = -24; j < 25; j++) {
        G4double backGrid = distancePerGrid/2. - wireRadius;
        new G4PVPlacement(0, G4ThreeVector(0., 0, backGrid), fWireGridLogical[j + 24][i],
                          name, fGridLogical[i], false, j + 24 + 49 + 49, checkOverlaps);
      }
    }
  }

  // BC-400 Plastic Scintillator
  G4VSolid* scintSolid = new G4Tubs("scintSolid", 0., scintRadius, scintillatorThickness/2., 0., 360.*deg);
  fScintLogical =  new G4LogicalVolume(scintSolid, BC400, "scintLogical");
  new G4PVPlacement(0, G4ThreeVector(0., 0., scintillatorDetectPos), fScintLogical, "scintPhysical", fDetectLogical,
                    false, 0, checkOverlaps);

  SetAttributes();

  return worldPhysical;
}

void MMDetectorConstruction::ConstructMaterials() {
  G4NistManager* man = G4NistManager::Instance();

  man->ConstructNewGasMaterial("Methane", "G4_METHANE", fTemperature*kelvin,
                               fPressureInTorr*1.333e-3*bar);
  man->ConstructNewGasMaterial("CO2", "G4_CARBON_DIOXIDE", fTemperature*kelvin,
                               fPressureInTorr*1.333e-3*bar);
}

void MMDetectorConstruction::ConstructSDandField() {
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String SDname;

  // Target
  char nameTarget[] = "target";
  G4VSensitiveDetector* targetDetector = new MMGenSD(SDname = nameTarget);
  SDman->AddNewDetector(targetDetector);
  fTargetLogical->SetSensitiveDetector(targetDetector);

  // Foil
  char nameFoil[] = "foil";
  G4VSensitiveDetector* foilDetector = new MMGenSD(SDname = nameFoil);
  SDman->AddNewDetector(foilDetector);
  fFoilLogical->SetSensitiveDetector(foilDetector);

  // IC Grids
  for(G4int i = 0; i < fNumGrids; i++) {
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

  for(G4int i = 0; i < fNumGrids; i++) {
    G4VisAttributes* gridAttr = new G4VisAttributes(gridColors[i]);
    gridAttr->SetVisibility(true);
    gridAttr->SetForceSolid(true);
    fGridLogical[i]->SetVisAttributes(gridAttr);
  }

  for(G4int i = 0; i < fNumGrids; i++) {
    G4VisAttributes* wireAttr = new G4VisAttributes(G4Colour::Red());
    wireAttr->SetVisibility(true);
    wireAttr->SetForceSolid(true);
    for(G4int j = 0; j < 49; j++) {
      fWireGridLogical[j][i]->SetVisAttributes(wireAttr);
    }
  }

  G4VisAttributes* scintAttr = new G4VisAttributes(G4Colour::Red());
  scintAttr->SetVisibility(true);
  scintAttr->SetForceSolid(true);
  fScintLogical->SetVisAttributes(scintAttr);
}
