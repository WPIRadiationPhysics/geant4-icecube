#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "PhysicsList.hh"
//#include "FTFP_BERT.hh"
#include "UltraPhysicsList.hh"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " ./IceCube [-n nEvents ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   Note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

int main(int argc,char** argv) {

  // Declare vars
  G4int nEvents = 0;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif

  // Evaluate arguments, don't allow breaking
  if ( argc > 9 ) { PrintUsage(); return 1; }  
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-n" ) nEvents = std::atoi(argv[i+1]);
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }  
  
  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  if ( nThreads > 0 ) { 
    runManager->SetNumberOfThreads(nThreads);
  }  
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  // Initialize mandatory classes
  DetectorConstruction* detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);
  //G4VModularPhysicsList* physicsList = new FTFP_BERT;
  UltraPhysicsList* physicsList = new UltraPhysicsList();
  runManager->SetUserInitialization(physicsList);
  ActionInitialization* actionInitialization = new ActionInitialization(detConstruction);
  runManager->SetUserInitialization(actionInitialization);
  runManager->Initialize();

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Define particle and energy=
  UImanager->ApplyCommand("/gun/particle kaon-"); // charged particle
 // UImanager->ApplyCommand("/gun/particle proton"); // charged particle
  UImanager->ApplyCommand("/gun/energy 7.665323 GeV"); // avg energy

  // Begin simulation
  if ( nEvents > 0 ) {

    // 1 micrometer particle track cuts
    UImanager->ApplyCommand("/run/setCut 0.01 mm");

    std::ostringstream syscmdStream; G4String syscmd;

    // Run experimental energies
    syscmdStream.str(""); syscmdStream << "/run/beamOn " << nEvents;
    syscmd = syscmdStream.str(); UImanager->ApplyCommand(syscmd);
    
  } else {

    // mm track cuts
    UImanager->ApplyCommand("/run/setCut 0.01 mm");

    // Initialize visualizer and gui macros
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv, session);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
    if (ui->IsGUI()) { UImanager->ApplyCommand("/control/execute gui.mac"); }
    ui->SessionStart();
    delete ui;
#endif
  }

  // Dump manager objects from memory
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
