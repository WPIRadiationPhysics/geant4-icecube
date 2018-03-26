#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"
#include "DetectorConstruction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
#include "G4EmCalculator.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>

RunAction::RunAction() : G4UserRunAction() {}

RunAction::~RunAction() { /* delete G4AnalysisManager::Instance(); */ }

void RunAction::BeginOfRunAction(const G4Run* run) {
	
    // Acquire analysis instance
    Analysis* simulationAnalysis = Analysis::GetAnalysis();

    // Set number of events for processing
    G4int numEvents = run->GetNumberOfEventToBeProcessed();
    simulationAnalysis->SetNumEvents(numEvents);

}

void RunAction::EndOfRunAction(const G4Run* /*run*/) {

    // Acquire analysis instance
    Analysis* simulationAnalysis = Analysis::GetAnalysis();
    G4int i, numEvents = simulationAnalysis->GetNumEvents();

    // Declare data filename    
    std::ostringstream fileNameStream;
    G4String fileName;
    fileNameStream << "datafile.txt";
    fileName = fileNameStream.str();
    
    // Export histogram values into data file
    std::ofstream theFile;
    theFile.open(fileName);
    theFile << "// Time [0.25 x ns], Average, StDev (" << numEvents << " events processed)" << G4endl;
    for (i=0; i<100; i++) {
		//G4double binTally = (float)(simulationAnalysis->callOpticalHistogram(i));
		//G4double binSquaredTally = (float)(simulationAnalysis->callSquaredOpticalHistogram(i));
		//G4double tallyAverage = binTally/((float)(numEvents));
		G4double tallyAverage = simulationAnalysis->callAverageOpticalHistogram(i);
		//G4double tallyStDev = pow(binSquaredTally - (pow(binTally, 2)/((float)(numEvents))), 0.5);
		G4double tallyVar = simulationAnalysis->callAverageOpticalHistogram(i);
		G4double tallyStDev = pow(tallyVar, 0.5);
		theFile << tallyAverage << " " << tallyStDev << " " << G4endl;
	}
    theFile.close();
}
