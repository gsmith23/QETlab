// *******************************************************************
// * License and Disclaimer                                          *
// *                                                                 *
// * This software is copyright of Geant4 Associates International   *
// * Ltd (hereafter 'G4AI'). It is provided under the terms and      *
// * conditions described in the file 'LICENSE' included in the      *
// * software system.                                                *
// * Neither the authors of this software system nor G4AI make any   *
// * representation or warranty, express or implied, regarding this  *
// * software system or assume any liability for its use.            *
// * Please see the file 'LICENSE' for full disclaimer and the       *
// * limitation of liability.                                        *


// *******************************************************************
//test
#include "Tangle2DetectorConstruction.hh"
#include "Tangle2Data.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"



Tangle2DetectorConstruction::Tangle2DetectorConstruction()
: G4VUserDetectorConstruction(),
  fCheckOverlaps(true)
{
  DefineMaterials();
}

Tangle2DetectorConstruction::~Tangle2DetectorConstruction()
{ }

void Tangle2DetectorConstruction::DefineMaterials()
{

  G4double a; // mass of a mole
  G4double z; // mean number of protons
  G4String name, symbol;

  a=16.00*g/mole;
  G4Element*  elO = new G4Element(name="Oxygen", symbol="O", z=8., a);

  a=28.09*g/mole;
  G4Element*  elSi = new G4Element(name="Silicon", symbol="Si", z=14., a);
  a=174.97*g/mole;
  G4Element* elLu = new G4Element(name="Lutetium", symbol="Lu", z=71., a);

  G4Material* LSO;

  G4double density = 7.4*g/cm3;
  LSO = new G4Material("Lu2SiO5",   
                       density,    
                       3);         

  //Add Element for Material "Lu2SiO5"
  LSO->AddElement(elSi, 1);
  LSO->AddElement(elLu, 2);
  LSO->AddElement(elO , 5);  


  // Dump the Table of registered materials 
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* Tangle2DetectorConstruction::Construct()
{  
  G4NistManager* nist = G4NistManager::Instance();
  
  //Crystal parameters
  
  G4double cryst_dX = 22*mm, cryst_dY = 4*mm, cryst_dZ = 3*mm;

  G4Material* cryst_mat   = nist->FindOrBuildMaterial("Lu2SiO5");

  G4bool checkOverlaps = true;
  
  // World
  G4double world_sizeX = 12*cm;
  G4double world_sizeYZ  = 2*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       
       0.5*world_sizeX, 0.5*world_sizeYZ, 0.5*world_sizeYZ);
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          
                        world_mat,          
                        "World");           
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     
                      G4ThreeVector(),       
                      logicWorld,            
                      "World",               
                      0,                     
                      false,                 
                      0,                     
                      checkOverlaps);        
                     
  // Crystal
  G4double pos_dX = 0.5*(world_sizeX-cryst_dX);
  
  G4Box* solidCryst =    
    new G4Box("crystal",                    
	      0.5*cryst_dX, 0.5*cryst_dY, 0.5*cryst_dZ); 
      
  G4LogicalVolume* logicCryst =                         
    new G4LogicalVolume(solidCryst,            
                        cryst_mat,             
                        "CrystalLV");         
               
  
  //array of positions for 18 crystals
  G4int nb_cryst = 18;
  
  G4ThreeVector positions[18] = {
    G4ThreeVector(pos_dX,-cryst_dY,cryst_dZ),
    G4ThreeVector(pos_dX,0,cryst_dZ),
    G4ThreeVector(pos_dX,cryst_dY,cryst_dZ),
    G4ThreeVector(pos_dX,-cryst_dY,0),
    G4ThreeVector(pos_dX,0,0),
    G4ThreeVector(pos_dX,cryst_dY,0),
    G4ThreeVector(pos_dX,-cryst_dY,-cryst_dZ),
    G4ThreeVector(pos_dX,0,-cryst_dZ),
    G4ThreeVector(pos_dX,cryst_dY,-cryst_dZ),
    G4ThreeVector(-pos_dX,-cryst_dY,cryst_dZ),
    G4ThreeVector(-pos_dX,0,cryst_dZ),
    G4ThreeVector(-pos_dX,cryst_dY,cryst_dZ),
    G4ThreeVector(-pos_dX,-cryst_dY,0),
    G4ThreeVector(-pos_dX,0,0),
    G4ThreeVector(-pos_dX,cryst_dY,0),
    G4ThreeVector(-pos_dX,-cryst_dY,-cryst_dZ),
    G4ThreeVector(-pos_dX,0,-cryst_dZ),
    G4ThreeVector(-pos_dX,cryst_dY,-cryst_dZ)};

  for (G4int icrys = 0; icrys < nb_cryst; icrys++) {
       new G4PVPlacement(0,                      
			 positions[icrys],       
			 logicCryst,             
			 "crystal",              
			 logicWorld,             
			 false,                  
			 icrys,
			 checkOverlaps);          
  }
  
  return physWorld;
}
