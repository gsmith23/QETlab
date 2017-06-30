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

#include "Tangle2PrimaryGeneratorAction.hh"

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
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="gamma");

  //photon1
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(511*keV);
  G4double x0  = 0*cm, y0  = 0*cm, z0  = 0*cm;
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun->SetParticlePolarization(G4RandomDirection());
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1,0,0));
  //create vertex
  fParticleGun->GeneratePrimaryVertex(anEvent);

  //photon 2
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(511*keV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(-1,0,0));
  fParticleGun->SetParticlePosition(G4ThreeVector());
  fParticleGun->SetParticlePolarization(G4RandomDirection());
  //create vertex
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

