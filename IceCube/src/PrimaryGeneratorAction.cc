#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <math.h>

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0),
   fIceBox(0) {

  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  // Find geometry-defined length to orient gun, spit error and center if not found
  if (!fIceBox) {
    G4LogicalVolume* IceBoxLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("IceCubeBox");
    if ( IceBoxLV ) fIceBox = dynamic_cast<G4Box*>(IceBoxLV->GetSolid());
  }
  // Place gun side of universe
  fParticleGun->SetParticlePosition(G4ThreeVector());
  if ( !fIceBox ) { 
  //  G4double IceBoxHalfLength = fIceBox->GetXHalfLength();
  //  // Replace gun at 2 * side length from origin
  //  
  //}
  //else  {
    G4ExceptionDescription msg;
    msg << "IceBox volume of box shape not found.\n";
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("B1PrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }
  // Define neutrino interaction product flux statistics (from GENIE)
   G4double averagePxProton = -0.00422, sigmaPxProton = 1.492389,
            averagePyProton = 0.005388, sigmaPyProton = 1.486205,
            averagePzProton = .999977, sigmaPzProton = 23.10851,
            averageEProton = 1.977057,  sigmaEProton = 56.52311,
            averagePxAntiProton = -0.00291, sigmaPxAntiProton = 1.170056,
            averagePyAntiProton = 0.002401, sigmaPyAntiProton = 1.353307,
            averagePzAntiProton = .9999993, sigmaPzAntiProton = 55.345521,
            averageEAntiProton = 9.6120611,  sigmaEAntiProton = 577.7428,
            averagePxPiplus = -0.00542, sigmaPxPiplus = 0.467716,
            averagePyPiplus = 0.009647, sigmaPyPiplus = 0.470216,
            averagePzPiplus = 0.999938, sigmaPzPiplus = 16.91436,
            averageEPiplus = 6.66052,  sigmaEPiplus = 114.2266,
            averagePxPiminus = -0.00406, sigmaPxPiminus = 0.0646371,
            averagePyPiminus = 0.007413, sigmaPyPiminus = 0.707254,
            averagePzPiminus = 0.999964, sigmaPzPiminus = 17.88839,
            averageEPiminus = 4.91014,  sigmaEPiminus = 89.44806,
            averagePxTau = 0.002669, sigmaPxTau = 0.40388,
            averagePyTau = -0.0075, sigmaPyTau = 0.356579,
            averagePzTau = 0.999968, sigmaPzTau = 16.21363,
            averageETau =51.60144,  sigmaETau = 864.873,
            averagePxLambdaCharmedPlus = 0.019345, sigmaPxLambdaCharmedPlus = 2.037889,
            averagePyLambdaCharmedPlus = -0.00798, sigmaPyLambdaCharmedPlus = 1.094753,
            averagePzLambdaCharmedPlus = 0.999781, sigmaPzLambdaCharmedPlus = 54.00723,
            averageELambdaCharmedPlus = 22.93114,  sigmaELambdaCharmedPlus = 1365.164,
            averagePxKaonPlus = 0.005517, sigmaPxKaonPlus = 1.666119,
            averagePyKaonPlus = 0.001942, sigmaPyKaonPlus = 1.737373,
            averagePzKaonPlus = 0.999834, sigmaPzKaonPlus = 31.7155,
            averageEKaonPlus = 12.1808,  sigmaEKaonPlus = 400.4078,
            averagePxKaonMinus = 0.002367, sigmaPxKaonMinus = 1.593622,
            averagePyKaonMinus = 0.18082, sigmaPyKaonMinus = 1.771179,
            averagePzKaonMinus = 0.999834, sigmaPzKaonMinus = 41.99002,
            averageEKaonMinus = 7.6655323,  sigmaEKaonMinus = 341.3084;

  // Construct firing position and direction
  fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -0.5*m));
  //G4double beamPx = G4RandGauss::shoot(averagePxTau, sigmaPxTau/pow(2, 0.5)),
  //         beamPy = G4RandGauss::shoot(averagePyTau, sigmaPyTau/pow(2, 0.5)),
  //         beamPz = G4RandGauss::shoot(averagePzTau, sigmaPzTau/pow(2, 0.5));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(beamPx, beamPy, beamPz));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(averagePxTau, averagePyTau, averagePzTau));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(averagePxKaonMinus, averagePyKaonMinus, averagePzKaonMinus));

  // Fire
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
