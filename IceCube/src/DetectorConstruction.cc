#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Orb.hh" 
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0; 

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fDetector(0),
   fCheckOverlaps(true) {

     // Acquire material data
     DefineMaterials();
}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {

  // Define geometric volumes
  return DefineVolumes();
}

void DetectorConstruction::DefineMaterials() {
  // Custom Definition of ice
  G4double ice_density     = 0.9761*g/cm3;  // from Wikipedia
  G4double ice_pressure    = 101325*pascal; // 1 atm
  G4double ice_temperature = 273.15*kelvin;    // 0 deg C
  //G4double ice_molarmass   = 18.0153*g/mole; // from WolframAlpha
  G4Material* Ice = new G4Material ("Ice", ice_density, 2, kStateUndefined, ice_temperature,  ice_pressure);
  G4Element* elH = new G4Element("Hydrogen", "H", 1, 0.9169*g/mole);
  G4Element* elO = new G4Element("Oxygen", "O", 8, 16.00*g/mole);
  Ice->AddElement(elH, 2);
  Ice->AddElement(elO, 1);
  
  const G4int NUMENTRIES = 11;
  G4double ppckov[NUMENTRIES] = { 1.771*eV, 1.851*eV, 1.937*eV, 2.033*eV, 2.138*eV, 2.254*eV, 2.384*eV, 2.530*eV, 2.695*eV, 2.883*eV, 3.099*eV };
  G4double rindex[NUMENTRIES] = { 1.306, 1.307, 1.308, 1.309, 1.310, 1.311, 1.313, 1.313, 1.315, 1.317, 1.319  };
  G4double absorption[NUMENTRIES] = { 192.0*cm, 282.1*cm, 417.4*cm, 704.5*cm, 1142.4*cm, 1407.3*cm, 1830.9*cm, 2190.6*cm, 2392.5*cm, 1645.1*cm, 1174.5*cm };
  G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
  MPT->AddProperty("RINDEX", ppckov, rindex, NUMENTRIES);
  MPT->AddProperty("ABSLENGTH", ppckov, absorption, NUMENTRIES);
  Ice->SetMaterialPropertiesTable(MPT);
	
  // Materials defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_WATER");
  
  
  
  // Geant4 conventional definition of a vacuum
  G4double vacuum_density     = universe_mean_density;  //from PhysicalConstants.h
  G4double vacuum_pressure    = 1.e-19*pascal;
  G4double vacuum_temperature = 0.1*kelvin;
  G4double vacuum_molarmass   = 1.01*g/mole; 
  new G4Material("Vacuum", 1., vacuum_molarmass, vacuum_density,
                   kStateGas, vacuum_temperature, vacuum_pressure);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//// Geometry parameters
G4VPhysicalVolume* DetectorConstruction::DefineVolumes() {

  // Declarations
  G4double IceCubeLength = 1*m;                       // side length of bulk ice

  G4double IceCubeHalfLength = 0.5*IceCubeLength;     // half side length of bulk ice
  G4double worldBoxHalfLength = 2*IceCubeHalfLength; // half side length of world
  G4double DomRadius = 10*2.54*cm/2; 
  // Get materials
  G4Material* defaultMaterial = G4Material::GetMaterial("Vacuum");
  G4Material* iceMaterial = G4Material::GetMaterial("Ice");
  G4Material* DOMMaterial = G4Material::GetMaterial("Ice");

  // Throw exception to ensure material usability
  if ( ! defaultMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }

  // World box
  // Box solid definition
  G4VSolid* worldS 
    = new G4Box(
            "worldBox",      // its name
            worldBoxHalfLength,  // parameters
            worldBoxHalfLength,
            worldBoxHalfLength
            );

  // World volume
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
            worldS,          // its solid
            defaultMaterial, // its material
            "world"          // its name
            );

  // World physical volume of solid placed in LV
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
            0,                // no rotation
            G4ThreeVector(),  // no translation
            worldLV,          // its logical volume                         
            "worldLV",          // its name
            0,                // its mother  volume
            false,            // no boolean operation
            0,                // copy number
            fCheckOverlaps    // checking overlaps
            );
  // World visualization attributes
  G4VisAttributes* defaultVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  defaultVisAtt->SetVisibility(true);
  worldLV->SetVisAttributes(defaultVisAtt);

  // IceCube box
  // Box solid definition
  G4VSolid* IceCubeS 
    = new G4Box(
            "IceCubeBox",   // its name
            IceCubeHalfLength,  // parameters
            IceCubeHalfLength,
            IceCubeHalfLength
            );

  // IceCube volume
  G4LogicalVolume* IceCubeLV
    = new G4LogicalVolume(
            IceCubeS,          // its solid
            iceMaterial,     // its material
            "IceCubeBox"         // its name
            );

  // IceCube Physical volume of solid placed in LV
  G4VPhysicalVolume* IceCubePV
    = new G4PVPlacement(
            0,                // no rotation
            G4ThreeVector(),  // no translation (from center of mother volume)
            IceCubeLV,        // its logical volume                         
            "IceCubeLV",        // its name
            worldLV,          // its mother  volume
            false,            // no boolean operation
            0,                // copy number
            fCheckOverlaps    // checking overlaps
            );


  //DOM Construction
  G4VSolid* IceCubeDOMS
   = new G4Orb(
			"DOM1",
            DomRadius);
            
  //DOM Volume 
  G4LogicalVolume* IceCubeDOMLV
  =new G4LogicalVolume(
			IceCubeDOMS,
			DOMMaterial, 
			"Dom1"
			);

  // DOM Physical volume of solid placed in LV
    G4VPhysicalVolume* IceCubeDOMPV
    = new G4PVPlacement(
            0,                
            G4ThreeVector(),  
            IceCubeDOMLV,                                 
            "IceCubeDOMLV",        
            IceCubeLV,          
            false,            
            0,                
            fCheckOverlaps    
            );
 
  
             

  // IceCube Visualization attributes
  G4VisAttributes* iceVisAtt = new G4VisAttributes(G4Colour(0, 0, 1.0));
  iceVisAtt->SetVisibility(true);
  IceCubeLV->SetVisAttributes(iceVisAtt);
  G4VisAttributes* DOMVisAtt = new G4VisAttributes(G4Colour(0, 1.0, 0));
  DOMVisAtt->SetVisibility(true);
  IceCubeDOMLV->SetVisAttributes(DOMVisAtt);

  // Always return the physical World
  return worldPV;
}
