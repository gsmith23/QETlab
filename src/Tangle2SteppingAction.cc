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

#include "Tangle2SteppingAction.hh"

#include "Tangle2RunAction.hh"
#include "Tangle2Data.hh"

#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4MTRunManager.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Gamma.hh"
#include "G4Threading.hh"
#include <vector>

Tangle2SteppingAction::Tangle2SteppingAction
(Tangle2RunAction* runAction)
{}

void Tangle2SteppingAction::BeginOfEventAction()
{
}

void Tangle2SteppingAction::EndOfEventAction()
{
}

//Define a function for calculating angles theta and phi
void CalculateThetaPhi
  (const G4ThreeVector& v,
   const G4ThreeVector& z_axis,
   // Output quantities
   G4ThreeVector& y_axis,
   G4ThreeVector& x_axis,
   G4double& cosTheta,
   G4double& theta,
   G4double& phi)
  {
    cosTheta = v*z_axis;
    theta = std::acos(cosTheta) * 180/(pi); //convert to degrees
    // Make y' perpendicular to global z-axis.
    y_axis = (z_axis.cross(G4ThreeVector(0,0,1))).unit();
    x_axis = y_axis.cross(z_axis);
    const G4ThreeVector ontoXYPlane = v.cross(z_axis);
    // ontoXYPlane is a vector in the xy-plane, but perpendicular to the
    // projection of the scattered photon, so
    const G4double projection_x = -ontoXYPlane*y_axis;
    const G4double projection_y = ontoXYPlane*x_axis;
    phi = std::atan2(projection_y,projection_x) * 180/(pi); //convert to degrees
  }


//initialise parameters 
G4int paramA, paramB,StepANo1, StepANo2, StepBNo1, StepBNo2, IdA, IdB;


void Tangle2SteppingAction::UserSteppingAction(const G4Step* step)
{
  
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4StepPoint* postStepPoint = step->GetPostStepPoint();

  G4double eDep = step->GetTotalEnergyDeposit();

  //G4VPhysicalVolume* prePV = preStepPoint->GetPhysicalVolume();
  G4VPhysicalVolume* postPV = postStepPoint->GetPhysicalVolume();

  G4ThreeVector prePos = preStepPoint->GetPosition();
  G4ThreeVector postPos = postStepPoint->GetPosition();

  G4ThreeVector preMomentumDir = preStepPoint->GetMomentumDirection();
  G4ThreeVector postMomentumDir = postStepPoint->GetMomentumDirection();

  G4Track* track = step->GetTrack();
  const G4VProcess* processDefinedStep = postStepPoint->GetProcessDefinedStep();
   
  G4int stepNumber = track->GetCurrentStepNumber();
  G4int ID = track->GetTrackID();
  G4String particleName = track->GetDefinition()->GetParticleName();
  G4String processName = processDefinedStep->GetProcessName();
  //G4ParticleDefinition* particleDefinition = track->GetDefinition();
  //const G4VProcess* creatorProcess = track->GetCreatorProcess();

    
  //Fill energy array
  if ((postPV) && (eDep > 0) && (postPV->GetName() != "disc") && (postPV->GetName() != "Coll_right") && (postPV->GetName() != "Coll_left")){
    Tangle2::eDepCryst[postPV->GetCopyNo()] += eDep;}

  //Fill Collimator energy depositions
  /* if((postPV->GetCopyNo()==18) && (eDep>0)){
    Tangle2::eDepColl1 +=eDep;}

  if((postPV->GetCopyNo()==19) && (eDep>0)){
    Tangle2::eDepColl2 +=eDep;}
  */



  //For the first particle and first step paramA1/B1 are set to zero
  //These will be used to determine when the FIRST Compton event 
  if((ID==2)&&(stepNumber==1)){
    paramA = 0;
    paramB = 0;
  }


  //-------------Look only at PHOTONS undergoing COMPTON processes--------------
  
  // return if particle is NOT a photon and the interaction is NOT Compton 
  
  if((particleName != "gamma") || (processName != "compt")) {
      return;
    }
  

  if((postPos[0]>0)){ 
    
   IdA =  ID;
    
   }

    
  if(postPos[0]<0){ 
    
    IdB = ID;
      
    }
  


  
  //------Determine Step Numbers of relevant interactions----------------
    
  /*  Find the step number for the FIRST Compton interaction 
      occurs when paramA/paramB equal 0
      At this interaction set paramA/paramB to be 1
      
      Then find the step number for the SECOND Compton interaction
      occurs when paramA/paramB equal 1
      Set paramA/paramB to be 2, so no further Compton interactions are included */
  
  //Photon 1
  if((ID == IdA) && (paramA == 0)){ //first compton event for A
     paramA = 1;
     StepANo1 = stepNumber;
   }
   
  if ((ID == IdA) && (stepNumber>StepANo1) && (paramA==1)){ //second compton event for A
     paramA = 2;
     StepANo2 = stepNumber;
  }

  //Photon 2
   if((ID == IdB) && (paramB ==0)){ //first compton event for B
     paramB = 1;
     StepBNo1 = stepNumber;
   }
   
   if ((ID == IdB) && (stepNumber>StepBNo1) && (paramB==1)){ //second compton event for B
     paramB = 2;
     StepBNo2 = stepNumber;
  }

 
    
  //-----------------Photon 1----------------
   // if (ID == IdA){
   if(postPos[0]>0){ //photon has hit detector A
  //first interaction
  if (stepNumber == StepANo1){
      
    Tangle2::posA_1 = postPos; 
      
    //calculate the angles 
    G4ThreeVector photon1_y_axis;  // dummy, i.e., not used.
    G4ThreeVector photon1_x_axis;  // dummy
    G4double fCosTheta1;
    
    CalculateThetaPhi(postMomentumDir,
		      preMomentumDir,
		      photon1_y_axis,
		      photon1_x_axis,
		      fCosTheta1,
		      Tangle2::thetaA,
		      Tangle2::phiA);

      if (Tangle2::thetaA==0){
	G4cout<<"theta A is zero"<<G4endl;
      }

  }
      
  

    // second interaction
   if (stepNumber == StepANo2){
  
    Tangle2::posA_2 = postPos;
    
     //calculate the angles 
    G4ThreeVector photon1_y_axis2;  // dummy, i.e., not used.
    G4ThreeVector photon1_x_axis2;  // dummy
    G4double fCosThetaA2;
    
    CalculateThetaPhi(postMomentumDir,
		      preMomentumDir,
		      photon1_y_axis2,
		      photon1_x_axis2,
		      fCosThetaA2,
		      Tangle2::thetaA2,
		      Tangle2::phiA2);
    }
    
  }
   
  //---------------------Photon 2-------------------------
   //if (ID == IdB){
   if(postPos[0]<0){ //photon has hit detector B
  //first interaction
	if (stepNumber == StepBNo1){
   
    Tangle2::posB_1 = postPos;
    
	//calculate angles 
    G4ThreeVector photon2_y_axis;  // dummy, i.e., not used.s
    G4ThreeVector photon2_x_axis;  // dummy
    G4double fCosTheta2; 
    CalculateThetaPhi(postMomentumDir,
		      preMomentumDir,
		      //G4ThreeVector(1,0,0),
		      photon2_y_axis,
		      photon2_x_axis,
		      fCosTheta2,
		      Tangle2::thetaB,
		      Tangle2::phiB);
    
     if (Tangle2::thetaB==0){
	G4cout<<"theta B is zero"<<G4endl;
      }
    
  }
  
  //second interaction
	if (stepNumber == StepBNo2){
    
    Tangle2::posB_2 = postPos;

    
	//calculate angles 
    G4ThreeVector photon2_y_axis2;  // dummy, i.e., not used.s
    G4ThreeVector photon2_x_axis2;  // dummy
    G4double fCosThetaB2; 
    CalculateThetaPhi(postMomentumDir,
		      preMomentumDir,
		      //G4ThreeVector(1,0,0),
		      photon2_y_axis2,
		      photon2_x_axis2,
		      fCosThetaB2,
		      Tangle2::thetaB2,
		      Tangle2::phiB2);

	}

  }
  
   
//Count the number of compton scatters occuring in each respective crystal

   for (G4int i = 0; i<9; i++){

     if((stepNumber==StepANo1) && (ID==IdA)){
	 Tangle2::nb_Compt[i]=0; //set to zero at the beginning of each event
	}
    
     if(postPV->GetCopyNo()==i){
	Tangle2::nb_Compt[i] +=1;
       }
     }

   for (G4int i = 9; i<18; i++){
	
     if((stepNumber==StepBNo1) && (ID==IdB)){
	  Tangle2::nb_Compt[i]=0; //set to zero at the beginning of each event
	}
    
     if(postPV->GetCopyNo()==i){
	  Tangle2::nb_Compt[i] +=1;
	}
   }
	
  

 
   //--Calculate delta phi--
   
   Tangle2::dphi = Tangle2::phiB + Tangle2::phiA;

   //Enforce 0<dphi<360 
   if (Tangle2::dphi <0){
     Tangle2::dphi = Tangle2::dphi + 360;}
 



return;
}

