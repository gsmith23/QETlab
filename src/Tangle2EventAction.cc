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

#include "Tangle2EventAction.hh"

#include "Tangle2Data.hh"
#include "Tangle2RunAction.hh"
#include "Tangle2VSteppingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include "G4Event.hh"

Tangle2EventAction::Tangle2EventAction
(Tangle2VSteppingAction* onePhotonSteppingAction)
: fpTangle2VSteppingAction(onePhotonSteppingAction)
{}

Tangle2EventAction::~Tangle2EventAction()
{
delete G4AnalysisManager::Instance();
}

void Tangle2EventAction::BeginOfEventAction(const G4Event*)
{
  fpTangle2VSteppingAction->BeginOfEventAction();
  for (G4int i = 0; i<18; i++){
    Tangle2::eDepCryst[i] = 0.;}
  Tangle2::nb_Compt = 0;
  Tangle2::posA = G4ThreeVector();
  Tangle2::posB = G4ThreeVector();
}

void Tangle2EventAction::EndOfEventAction(const G4Event*)
{   
  fpTangle2VSteppingAction->EndOfEventAction();
  
  // Always use a lock when writing a file in MT mode
  G4AutoLock lock(&Tangle2::outFileMutex);
  

  //Check if the event is a 'true lab event'
  //that means exactly 4 fired crystals:
  //the cetral one and one outer one for each array

  G4int nb_HitsA = 0;
  G4int nb_HitsB = 0;
  G4double eDepEvent = 0.;
  G4double eThres = 1*MeV; //in MeV

  //Array A
  for (G4int i = 0; i< 9 ; i++){
    if (Tangle2::eDepCryst[i] > 0){
      nb_HitsA += 1;
      eDepEvent += Tangle2::eDepCryst[i];}
   }
  //Array B
  for (G4int i = 9; i< 18; i++){
   if (Tangle2::eDepCryst[i] > 0){
      nb_HitsB += 1;
      eDepEvent += Tangle2::eDepCryst[i];}
   }

  //Output only 'true lab events' to the root file
  //
  //'True QE event' should have nb_Compt = 2
  //AND unit vectors posA and posB
  //if ((nb_HitsA == 2) && (nb_HitsB == 2) && (eDepEvent > eThres)){
  if ((nb_HitsA >= 1) && (nb_HitsB >= 1) && (eDepEvent > eThres)){
      G4AnalysisManager* man = G4AnalysisManager::Instance();
      for (G4int i = 0 ; i < 18 ; i++){
	man->FillNtupleDColumn(i, Tangle2::eDepCryst[i]/MeV);}
      man->FillNtupleIColumn(18, Tangle2::nb_Compt);
      man->FillNtupleDColumn(19, Tangle2::posA[0]/mm);
      man->FillNtupleDColumn(20, Tangle2::posA[1]/mm);
      man->FillNtupleDColumn(21, Tangle2::posA[2]/mm);
      man->FillNtupleDColumn(22, Tangle2::posB[0]/mm);
      man->FillNtupleDColumn(23, Tangle2::posB[1]/mm);
      man->FillNtupleDColumn(24, Tangle2::posB[2]/mm);
      
      man->AddNtupleRow();
  }

  //Record the number of events with 511 keV deposited in the central crystals
  if ((nb_HitsA == 1) && (nb_HitsB == 1) && (eDepEvent > eThres)){
    Tangle2::nEventsPh += 1;
  }
  
}
