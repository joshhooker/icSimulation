#include "MMActionInitialization.hh"
#include "MMAnalysis.hh"
#include "MMDetectorConstruction.hh"

#include "QGSP_BERT.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"
#include "time.h"

int main(int argc,char** argv)
{
  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  //set random seed with system time
  G4long seed = time(NULL);
  if(argc>2) seed += 473879*atoi(argv[2]);
  CLHEP::HepRandom::setTheSeed(seed);

  MMAnalysis* analysis = MMAnalysis::Instance();
  if(argc>2) {
    char name[10];
    char prefix[20];
    if(argc>3) sprintf(prefix, "%s", argv[3]);
    else sprintf(prefix, "sim");
    sprintf(name,"%s_%d.root", prefix, atoi(argv[2]));
    analysis->SetFilename(name);
  } else {
    analysis->SetFilename("sim.root");
  }

  G4double cmEnergy = (argc>4) ? atof(argv[4]) : 0.;

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Mandatory user initialization classes
  runManager->SetUserInitialization(new MMDetectorConstruction);

  G4VModularPhysicsList* physicsList = new QGSP_BERT;
  // ProtonScatteringPhysics* scatteringPhysics = new ProtonScatteringPhysics;
  // scatteringPhysics->SetCMScatteringEnergy(cmEnergy);
  // physicsList->RegisterPhysics(scatteringPhysics);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new MMActionInitialization());

  // Initialize Geant4 kernel
  runManager->Initialize();

  #ifdef G4VIS_USE
    // Visualization manager construction
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
  #endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc>1) {
      // execute an argument macro file if exist
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
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
