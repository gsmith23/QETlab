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
  
  G4int nComptonA,     nComptonB;
  G4int fstCompStep_A, sndCompStep_A;
  G4int fstCompStep_B, sndCompStep_B;
  
  G4int trackID_A1, trackID_B1;

  G4int previousEventID = -1;
  G4int eventID         = 0;
  
  G4bool doubleComptEvent = true;

  // To Do:
  // Investigate dphi calaulated
  // using LOR between hits
  
  // G4ThreeVector LOR;  
  
  // G4ThreeVector LOR_A1B2;  
  // G4ThreeVector LOR_A2B1;  
  // G4ThreeVector LOR_A2B2;  
  
  G4ThreeVector beam_A;  
  
  G4ThreeVector vScat_A1;
  G4double      cosTheta_A1;
  G4ThreeVector y_axis_A1; 
  G4ThreeVector x_axis_A1;
  
    
  G4ThreeVector vScat_A2;
  G4double      cosTheta_A2;  
  G4ThreeVector y_axis_A2; 
  G4ThreeVector x_axis_A2;
  
  G4ThreeVector beam_B;
  
  G4ThreeVector vScat_B1;
  G4double      cosTheta_B1;
  G4ThreeVector y_axis_B1; 
  G4ThreeVector x_axis_B1;
  
      
  G4ThreeVector vScat_B2;  
  G4double      cosTheta_B2;
  G4ThreeVector y_axis_B2; 
  G4ThreeVector x_axis_B2;
  
  G4bool comments = false;
  
  
};

#endif
