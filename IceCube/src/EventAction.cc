#include "EventAction.hh"
#include "RunAction.hh"
#include "Randomize.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>

// Useful module inserts
EventAction::EventAction() : G4UserEventAction() {}
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* /*event*/) {
	
	// Acquire analysis instance
        Analysis* simulationAnalysis = Analysis::GetAnalysis();

	// Re-zero event's optical histograms
	simulationAnalysis->resetEventOpticalHistogram();
	
}

void EventAction::EndOfEventAction(const G4Event* /*event*/) {
	
        // Acquire analysis instance
        Analysis* simulationAnalysis = Analysis::GetAnalysis();
        G4int numEvents = simulationAnalysis->GetNumEvents();

	// Move event tallies over to sum and squared sum histograms
	simulationAnalysis->tallyOpticalHistograms(numEvents);
}
