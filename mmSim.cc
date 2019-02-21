#include "MMActionInitialization.hh"
#include "MMAnalysis.hh"
#include "MMDetectorConstruction.hh"

#include "QGSP_BERT.hh"
#include "BinaryReactionPhysics.hh"

#include "G4RadioactiveDecayPhysics.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
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

  //CREATE AND READ JSON CONFIG
  Json::Value config;
  std::string configFileName = argv[1];
  std::ifstream configStream(configFileName.c_str());
  configStream >> config;
  configStream.close();

  //Parse JSON
  G4double gasPressure = config["gasPressure"].asDouble(); // Torr
  G4double gasTemperature = config["gasTemperature"].asDouble(); // K
  std::map<std::string, G4double> eventActionParams;
  eventActionParams["fanoFactor"] = config["fanoFactor"].asDouble();
  eventActionParams["workFunction"] = config["workFunction"].asDouble();
  G4int processNumber = config["processNumber"].asInt();
  G4String macroName = config["macroName"].asString();
  G4bool isInteractive = config["interactive"].asBool();
  std::map<std::string, G4int> reactionParams;
  reactionParams["qValue"] = config["qValue"].asDouble();
  reactionParams["lightProductCharge"] = config["lightProduct"][0].asInt();
  reactionParams["lightProductMass"] = config["lightProduct"][1].asInt();
  reactionParams["heavyProductCharge"] = config["heavyProduct"][0].asInt();
  reactionParams["heavyProductMass"] = config["heavyProduct"][1].asInt();
  reactionParams["targetCharge"] = config["target"][0].asInt();
  reactionParams["targetMass"] = config["target"][1].asInt();

  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  //set random seed with system time
  G4long seed = time(NULL);
  if(argc>2) seed += 473879*atoi(argv[2]);
  CLHEP::HepRandom::setTheSeed(seed);

  MMAnalysis* analysis = MMAnalysis::Instance();
  char name[10];
  sprintf(name, "sim_%d.root", processNumber);
  analysis->SetFilename(name);

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Mandatory user initialization classes
  MMDetectorConstruction* detector = new MMDetectorConstruction();
  detector->SetGasPressure(gasPressure);
  detector->SetGasTemperature(gasTemperature);
  runManager->SetUserInitialization(detector);

  G4VModularPhysicsList* physicsList = new QGSP_BERT;
  BinaryReactionPhysics* reactionPhysics = new BinaryReactionPhysics();
  reactionPhysics->SetReactionParams(reactionParams);
  physicsList->RegisterPhysics(reactionPhysics);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  MMActionInitialization* actionInit = new MMActionInitialization(detector);
  actionInit->SetEventActionParams(eventActionParams);
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

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

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
