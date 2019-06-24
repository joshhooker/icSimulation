#include "MMActionInitialization.hh"
#include "MMDetectorConstruction.hh"

#include "BinaryReactionPhysics.hh"
#include "NucleonStates.hh"
#include "QGSP_BERT.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4MTRunManager.hh"
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

  G4int numGrids         = config["numberGrids"].asInt();
  G4double gridSize      = config["gridSize"].asDouble();
  G4double gridRadius    = config["gridRadius"].asDouble();
  G4double gridDist      = config["gridDistance"].asDouble();
  G4double scintDist     = config["scintDistance"].asDouble();
  G4double wireThickness = config["wireThickness"].asDouble();
  G4bool useInches       = config["useInches"].asBool();

  G4double scintResolution     = config["scintResolution"].asDouble();
  G4double extraGridResolution = config["extraGridResolution"].asDouble();

  G4String macroName   = config["macroName"].asString();
  G4bool isInteractive = config["interactive"].asBool();

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

#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif

  for(G4int i = 1; i < argc; i++) {
    G4cout << argv[i] << G4endl;
#ifdef G4MULTITHREADED
    if(G4String(argv[i]) == "-t") {
      nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
    }
#endif
  }

  // Construct the default run manager
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  if(nThreads > 0) {
    runManager->SetNumberOfThreads(nThreads);
  }
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Mandatory user initialization classes
  MMDetectorConstruction* detector = new MMDetectorConstruction();
  detector->SetGasType(gasType);
  detector->SetGasTemperature(gasTemperature);
  detector->SetGasPressure(gasPressure);
  detector->SetNumGrids(numGrids);
  detector->SetUseInches(useInches);
  detector->SetGridSize(gridSize);
  detector->SetGridRadius(gridRadius);
  detector->SetGridDistance(gridDist);
  detector->SetScintDistance(scintDist);
  detector->SetWireThickness(wireThickness);
  runManager->SetUserInitialization(detector);

  NucleonStates* states = NucleonStates::Instance();

  G4VModularPhysicsList* physicsList = new QGSP_BERT(0);
  BinaryReactionPhysics* reactionPhysics = new BinaryReactionPhysics();
  reactionPhysics->SetNumGrids(numGrids);
  physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  physicsList->RegisterPhysics(reactionPhysics);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  MMActionInitialization* actionInit = new MMActionInitialization(detector);
  actionInit->SetFanoFactor(fanoFactor);
  actionInit->SetWorkFunction(workFunction);
  actionInit->SetNumGrids(numGrids);
  actionInit->SetScintillatorResolution(scintResolution);
  actionInit->SetGridResolution(extraGridResolution);
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
