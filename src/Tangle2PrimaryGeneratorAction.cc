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
// $Id$
// John Allison  22nd May 2017
// Gary Smith    27th Nov 2017

#include "Tangle2PrimaryGeneratorAction.hh"

#include "Tangle2Data.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ChargedGeantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"
#include "G4PhysicalConstants.hh"

#include "G4GeneralParticleSource.hh"


Tangle2PrimaryGeneratorAction::Tangle2PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
}

Tangle2PrimaryGeneratorAction::~Tangle2PrimaryGeneratorAction()
{
  delete fParticleGun; 
}

void Tangle2PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  // Use positrons or
  // back to back photons
  G4bool generatePositrons = false;
  if(Tangle2::positrons)
    generatePositrons = true;
  
  // For back to back photons
  // a fixed axis beam can be chosen
  G4bool generateFixedAxis = false; 
  
  if (Tangle2::fixedAxis){
    if(!generatePositrons)
      generateFixedAxis = true;
    else
      G4cout << " Invalid choice: positrons with fixed axis " << G4endl;
  }
  
  G4bool generatePerpPol = false;
  
  // For back to back photons
  // the relative polarisation
  // can be orthogonal or random
  if(Tangle2::perpPol){
    if(!generatePositrons) 
      generatePerpPol = true;
    else 
      G4cout << " Invalid choice: positrons with perp pol  " << G4endl;
  }

  // perperdicular polarisation 
  // on fixed axes - for use with
  // fixed beam in x direction
  G4bool generatePolYandZ = false;
  
  if(Tangle2::polYZ)
    generatePolYandZ = true;
  
  // vertex
  G4double x0  = 0*cm, y0  = 0*cm, z0  = 0*cm;
  
  //-----------------------Back to back photons------------------------ 
  if(!generatePositrons){
    
    G4ThreeVector beam_axis;
    if (generateFixedAxis){
      beam_axis.set(1,0,0);
    }
    else{ // isotropic over theta = [0,12] degrees
      
      G4double theta    = 999999.9;
      G4double cosTheta = 99999.9;
      G4double sinTheta = 9999.9;
      G4double phi      = 999.9;
      G4double thetaMax = 12.; // atan(6 mm / radius in mm)

      if(Tangle2::fullPET)
	thetaMax = 0.8;
	
      // thetaMax degrees is just outside of 
      // outer crystal outer edge
      while(theta > thetaMax ){
	// code from G4RandomDirection
	cosTheta  = 2.*G4UniformRand()-1.;
	G4double sinTheta2 = 1. - cosTheta*cosTheta;
	if( sinTheta2 < 0.)  sinTheta2 = 0.;
	sinTheta  = std::sqrt(sinTheta2); 
	phi       = twopi*G4UniformRand();
	theta = std::acos(cosTheta)* 180/(pi);
      }
      
      // G4cout << " theta = " << theta << G4endl;
      
      // theta wrt x-axis (fixed beam in x)
      beam_axis.set(cosTheta,
		    sinTheta*std::cos(phi),
		    sinTheta*std::sin(phi));
      
      beam_axis = beam_axis.unit(); 
      
      // G4cout << " beam_axis = (" << beam_axis.getX() 
      //     <<              "," << beam_axis.getY() 
      //     <<              "," << beam_axis.getZ()
      //     <<              ")" << G4endl;
      
    
    }
    
    //----------------------------------
    // Photon 1
    fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName="gamma"));
    fParticleGun->SetParticleEnergy(511*keV);
    
    fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
    fParticleGun->SetParticleMomentumDirection(beam_axis);
    
    G4ThreeVector random = G4RandomDirection();
    
    // Restrict to YZ plane
    // To do: double check 'randomness' of projection
    G4ThreeVector randomYZ = random.cross(beam_axis).unit();
    
    G4ThreeVector y_axis   = G4ThreeVector(0,1,0);
    
    if(!generatePolYandZ)
      fParticleGun->SetParticlePolarization(randomYZ);
    else
      fParticleGun->SetParticlePolarization(y_axis);
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
    
    //----------------------------------
    // Photon 2
    fParticleGun->SetParticleEnergy(511*keV);
    fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
    fParticleGun->SetParticleMomentumDirection(-beam_axis);

    // randomise second photon polarisation wrt first
    G4ThreeVector randomYZ_2 = G4RandomDirection().cross(beam_axis).unit();
    // 
    G4ThreeVector z_axis = G4ThreeVector(0,0,1);
    // perpendicular to first photon
    G4ThreeVector perpRandomYZ = beam_axis.cross(randomYZ).unit();
    
    if(!generatePerpPol)
      fParticleGun->SetParticlePolarization(randomYZ_2);
    else if(generatePolYandZ)
      fParticleGun->SetParticlePolarization(z_axis);
    else 
      fParticleGun->SetParticlePolarization(perpRandomYZ);
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
  } // end of: if(!generatePositrons){
  else { // if (generatePositrons)
    
    fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName="e+"));
    
    // To do: investigate positron range
    fParticleGun->SetParticleEnergy(0*keV);
    
    fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
    
    //create vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  
}

