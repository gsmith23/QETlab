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
    theta = std::acos(cosTheta) * 180/(pi);
    // Make y' perpendicular to global x-axis.
    y_axis = (z_axis.cross(G4ThreeVector(0,1,0))).unit();
    x_axis = y_axis.cross(z_axis);
    const G4ThreeVector ontoXYPlane = v.cross(z_axis);
    // ontoXYPlane is a vector in the xy-plane, but perpendicular to the
    // projection of the scattered photon, so
    const G4double projection_x = -ontoXYPlane*y_axis;
    const G4double projection_y = ontoXYPlane*x_axis;
    phi = std::atan2(projection_y,projection_x) * 180/(pi);
  }


G4int param1, param2, StepNo1, StepNo2;


void Tangle2SteppingAction::UserSteppingAction(const G4Step* step)
{
  
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4StepPoint* postStepPoint = step->GetPostStepPoint();

  G4double eDep = step->GetTotalEnergyDeposit();

  // G4VPhysicalVolume* prePV = preStepPoint->GetPhysicalVolume();
  G4VPhysicalVolume* postPV = postStepPoint->GetPhysicalVolume();

  G4ThreeVector prePos = preStepPoint->GetPosition();
  G4ThreeVector postPos = postStepPoint->GetPosition();

  G4ThreeVector preMomentumDir = preStepPoint->GetMomentumDirection();
  G4ThreeVector postMomentumDir = postStepPoint->GetMomentumDirection();

  G4Track* track = step->GetTrack();
  G4int stepNumber = track->GetCurrentStepNumber(); 
  // G4ParticleDefinition* particleDefinition = track->GetDefinition();
  // const G4VProcess* creatorProcess = track->GetCreatorProcess();

  const G4VProcess* processDefinedStep = postStepPoint->GetProcessDefinedStep();

  // G4cout<<"Step number "<<stepNumber<<" for "<<processDefinedStep->GetProcessName()<<" for particle " <<track->GetTrackID()<<G4endl;

  //Fill energy array
  if ((eDep > 0) && (postPV->GetName() != "disc")){
    (postPV? Tangle2::eDepCryst[postPV->GetCopyNo()] += eDep : eDep = 0);}


  //---------------------Look at COMPTON or PHOTOELECTRIC processes-------------------
  
  // return if the interaction is NOT Compton or Photoelectric

  if((processDefinedStep->GetProcessName() != "compt") && (processDefinedStep->GetProcessName() != "phot"))
    {
    return;  
    }
  
    
   /*  Find the step number for the second Compton/photoelectric interaction
       param1/2 is used to stop assigning value to StepNo1/2 after the second interaction
       (Note that the first interaction is always step number 2)    */
  
  //Photon 1
   if((track->GetTrackID() == 1) && (stepNumber==2)){
     param1 =0;
   }
   
   if ((track->GetTrackID() == 1) && (stepNumber>2) && (param1!=1)){
     param1 += 1;
     StepNo1 = stepNumber;
  }

  //Photon 2
   if((track->GetTrackID() == 2) && (stepNumber==2)){
     param2 =0;
   }
   
  if ((track->GetTrackID() == 2) && (stepNumber>2) && (param2!=1)){
      param2 += 1;
      StepNo2 = stepNumber;
  }
    
  //Photon 1 first interaction
  if ((track->GetTrackID() == 1) && (stepNumber == 2)){
      
    Tangle2::countA1 = 1; //parameter for determining which events are counted
    Tangle2::posA_1 = 0.5*(prePos + postPos); 
      
    //calculate the angles 
    G4ThreeVector photon1_y_axis;  // dummy, i.e., not used.
    G4ThreeVector photon1_x_axis;  // dummy
    G4double fCosTheta1, thetaA, phiA;
    
    CalculateThetaPhi(postMomentumDir,
		      preMomentumDir,
		      photon1_y_axis,
		      photon1_x_axis,
		      fCosTheta1,
		      thetaA,
		      phiA);
    
      Tangle2::thetaA = thetaA;
      Tangle2::phiA = phiA;
      
  }
  
    //Photon 1 second interaction
  if ((track->GetTrackID() == 1) && (stepNumber == StepNo1)){
    Tangle2::countA2 = 1; //parameter for determining which events are counted
    Tangle2::posA_2 = 0.5*(prePos + postPos);
    }
  
  
  //Photon 2 first interaction
  if ((track->GetTrackID() == 2)&& (stepNumber == 2)){
    Tangle2::countB1 = 1; //parameter for determining which events are counted
    Tangle2::posB_1 = 0.5*(prePos + postPos);
    
	//calculate angles 
    G4ThreeVector photon2_y_axis;  // dummy, i.e., not used.s
    G4ThreeVector photon2_x_axis;  // dummy
    G4double fCosTheta2, thetaB, phiB; 
    CalculateThetaPhi(postMomentumDir,
		      preMomentumDir,
		      //G4ThreeVector(1,0,0),
		      photon2_y_axis,
		      photon2_x_axis,
		      fCosTheta2,
		      thetaB,
		      phiB);
    
    Tangle2::thetaB = thetaB;	
    Tangle2::phiB = phiB;
    
  }
  
  //Photon 2 second interaction
  if ((track->GetTrackID() == 2)&& (stepNumber == StepNo2)){
    Tangle2::countB2 = 1; //parameter for determining which events are counted
    Tangle2::posB_2 = 0.5*(prePos + postPos);
  }
  
  


    
//Count the number of compton scatters occuring in each respective crystal
  if (processDefinedStep->GetProcessName() == "compt"){

     for (G4int i = 0; i<18; i++){

       if(stepNumber==2){
	 Tangle2::nb_Compt[i]=0;
	}
    
       if(postPV->GetCopyNo()==i){
	Tangle2::nb_Compt[i] +=1;
       }
     }
  } 

  
  
//Calculate difference in phis 
Tangle2::dphi = Tangle2::phiB - Tangle2::phiA;

//Enforce 0<dphi<360 
if (Tangle2::dphi <0){
  Tangle2::dphi = Tangle2::dphi + 360;}
  

  
  
return;
}

