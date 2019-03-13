#include "MMActionInitialization.hh"
#include "MMAnalysis.hh"
#include "MMDetectorConstruction.hh"

#include "BinaryReactionPhysics.hh"
#include "QGSP_BERT.hh"

#include "G4RadioactiveDecayPhysics.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VModularPhysicsList.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "json/json.h"

#include "Randomize.hh"
#include "time.h"

int main(int argc,char** argv)
{
  if(argc < 2) {
    std::cout << "Usage: mmSim config-file" << std::endl;
    return 0;
  }

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  //CREATE AND READ JSON CONFIG
  Json::Value config;
  std::string configFileName = argv[1];
  std::ifstream configStream(configFileName.c_str());
  configStream >> config;
  configStream.close();

  //Parse JSON
  G4double gasPressure    = config["gasPressure"].asDouble(); // Torr
  G4double gasTemperature = config["gasTemperature"].asDouble(); // K
  G4String gasType        = config["gasType"].asString();

  G4int numGrids      = config["numberGrids"].asInt();
  G4double gridDist   = config["gridDistance"].asDouble();
  G4double gridRadius = config["gridRadius"].asDouble();
  G4double scintDist  = config["distanceScint"].asDouble();
  G4bool useInches    = config["useInches"].asBool();

  G4double scintResolution   = config["scintResolution"].asDouble();
  G4double extraGridResolution = config["extraGridResolution"].asDouble();

  G4String macroName   = config["macroName"].asString();
  G4bool isInteractive = config["interactive"].asBool();

  G4double qValue          = config["qValue"].asDouble();
  G4int lightProductCharge = config["lightProduct"][0].asInt();
  G4int lightProductMass   = config["lightProduct"][1].asInt();
  G4int heavyProductCharge = config["heavyProduct"][0].asInt();
  G4int heavyProductMass   = config["heavyProduct"][1].asInt();
  G4int targetCharge       = config["target"][0].asInt();
  G4int targetMass         = config["target"][1].asInt();

  G4bool writeAllEvents = config["writeAllEvents"].asBool();

  // Work out fanoFactor and workFunction from gasType
  G4double fanoFactor;
  G4double workFunction;
  if(gasType == "P10" || gasType == "p10") {
    fanoFactor = 0.17;
    workFunction = 28.0;
  }
  else if(gasType == "CO2" || gasType == "co2") {
    fanoFactor = 0.33;
    workFunction = 34.2;
  }
  else if(gasType == "Methane" || gasType == "methane" || gasType == "METHANE" || gasType == "CH4") {
    fanoFactor = 0.26;
    workFunction = 29.1;
  }
  else if(gasType == "CF4" || gasType == "cf4") {
    fanoFactor = 0.2;
    workFunction = 33.8;
  }
  else {
    fanoFactor = 0.26;
    workFunction = 29.1;
  }

  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  //set random seed with system time
  G4long seed = time(NULL);
  if(argc>2) seed += 473879*atoi(argv[2]);
  CLHEP::HepRandom::setTheSeed(seed);

  MMAnalysis* analysis = MMAnalysis::Instance();
  char name[10];
  sprintf(name, "sim.root");
  analysis->SetFilename(name);

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Mandatory user initialization classes
  MMDetectorConstruction* detector = new MMDetectorConstruction();
  detector->SetGasType(gasType);
  detector->SetGasTemperature(gasTemperature);
  detector->SetGasPressure(gasPressure);
  detector->SetNumGrids(numGrids);
  detector->SetUseInches(useInches);
  detector->SetGridDistance(gridDist);
  detector->SetGridRadius(gridRadius);
  detector->SetDistScint(scintDist);
  runManager->SetUserInitialization(detector);

  G4VModularPhysicsList* physicsList = new QGSP_BERT(0);
  BinaryReactionPhysics* reactionPhysics = new BinaryReactionPhysics();
  reactionPhysics->SetQValue(qValue);
  reactionPhysics->SetTarget(targetCharge, targetMass);
  reactionPhysics->SetLightProduct(lightProductCharge, lightProductMass);
  reactionPhysics->SetHeavyProduct(heavyProductCharge, heavyProductMass);
  reactionPhysics->SetNumGrids(numGrids);
  physicsList->RegisterPhysics(reactionPhysics);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  MMActionInitialization* actionInit = new MMActionInitialization(detector);
  actionInit->SetFanoFactor(fanoFactor);
  actionInit->SetWorkFunction(workFunction);
  actionInit->SetNumGrids(numGrids);
  actionInit->SetScintillatorResolution(scintResolution);
  actionInit->SetGridResolution(extraGridResolution);
  actionInit->SetWriteAllEvents(writeAllEvents);
  runManager->SetUserInitialization(actionInit);

  // Initialize Geant4 kernel
  runManager->Initialize();

  #ifdef G4VIS_USE
    // Visualization manager construction
    // G4VisManager* visManager = new G4VisExecutive();
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
  #endif

  if(!isInteractive) {
    // execute an argument macro file if exists
    G4String command = "/control/execute ";
    G4String filename = macroName;
    UImanager->ApplyCommand(command + filename);
  }
  else {
    // start interactive session
    #ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    #ifdef G4VIS_USE
	    UImanager->ApplyCommand("/control/execute init_vis.mac");
    #else
	    UImanager->ApplyCommand("/control/execute init.mac");
    #endif
	  if(ui->IsGUI()) {
	    UImanager->ApplyCommand("/control/execute gui.mac");
      ui->SessionStart();
      delete ui;
    }
    #endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  #ifdef G4VIS_USE
    delete visManager;
  #endif
    delete runManager;

  return 0;
}
