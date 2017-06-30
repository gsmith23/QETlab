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

#ifndef Tangle2PrimaryGeneratorAction_hh
#define Tangle2PrimaryGeneratorAction_hh 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

class G4GeneralParticleSource;

class Tangle2PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  Tangle2PrimaryGeneratorAction();
  virtual ~Tangle2PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

private:
  G4GeneralParticleSource* fpParticleGun;
};

#endif
