#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "Analysis.hh"
#include "G4SteppingManager.hh"

#include "G4Step.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <math.h>
#include <stdio.h>

// Initialize Step Procedure
SteppingAction::SteppingAction( const DetectorConstruction* detectorConstruction,
                               EventAction* eventAction)
                               : G4UserSteppingAction(),
                                 fDetConstruction(detectorConstruction),
                                 fEventAction(eventAction) {}
SteppingAction::~SteppingAction() {}

// Step Procedure (for every step...)
void SteppingAction::UserSteppingAction(const G4Step* step) {
	
  // Get step info of particle
  //G4String processname = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  G4String particlename = step->GetTrack()->GetDefinition()->GetParticleName();
  //G4int stepnum = step->GetTrack()->GetCurrentStepNumber();
  //G4int parentnum = step->GetTrack()->GetParentID();
  //G4int tracknum = step->GetTrack()->GetTrackID();
  //G4cout << "step " << stepnum << " of track: " << tracknum << " from parent " << parentnum << " - " << particlename << " undergoing " << processname << " process." << G4endl;
  
  // Get name of volume at step location
  G4String volumeName = step->GetTrack()->GetVolume()->GetLogicalVolume()->GetName();
  
  //if ( particlename == "opticalphoton" && volumeName == "Dom1" ) {
	//step->GetTrack()->SetTrackStatus(fStopButAlive);
  //}

  G4double waveformTime = 25*ns;
  //G4cout << "Volume is: " << volumeName << G4endl;
  
  // If opticalphoton at end of its track
  if ( particlename == "opticalphoton" /* && step->GetTrack()->GetTrackStatus() != fAlive */ ) {
	  
	// Get name of volume at track origin (vertex)
    G4String volumeNameVertex = step->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
    
    // If began in ice and ended in DOM
    if ( volumeNameVertex == "IceCubeBox" && volumeName == "Dom1" ) {
		
      //G4cout << "Volume is: " << volumeName << ", vertex volume is " << volumeNameVertex << G4endl;
		
	  // Get time of incidence (since beginning of event)
	  G4double globalTime = step->GetTrack()->GetGlobalTime();
	  
	  // Round down to nearest histogram time bin (ignore delayed stats)
	  if ( globalTime < waveformTime ) {
	    G4int binTime = (globalTime/(waveformTime/100));
			
		// Acquire analysis instance
                Analysis* simulationAnalysis = Analysis::GetAnalysis();
  
		// Add to optical histogram
		simulationAnalysis->appendOpticalHistogram(binTime);
		//G4cout << "Time is: " << globalTime << ", bin is: " << binTime << G4endl;
	  }
	}
  }

}

