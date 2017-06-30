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

#ifndef AnnihilationPhotonsSteppingAction_hh
#define AnnihilationPhotonsSteppingAction_hh

#include "Tangle2VSteppingAction.hh"

#include "Tangle2RunAction.hh"
#include "G4ThreeVector.hh"

class Tangle2SteppingAction: public Tangle2VSteppingAction
{
public:
  Tangle2SteppingAction(Tangle2RunAction*);
  virtual void BeginOfEventAction();
  virtual void UserSteppingAction(const G4Step*);
  virtual void EndOfEventAction();

private:
  Tangle2RunAction* fpRunAction;
};

#endif
