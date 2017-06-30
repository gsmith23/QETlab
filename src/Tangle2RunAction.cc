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

#include "Tangle2RunAction.hh"
#include "Tangle2Data.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4Threading.hh"
#include "G4AutoLock.hh"
#include <cassert>
#include <fstream>

Tangle2RunAction* Tangle2RunAction::fpMasterRunAction = 0;

namespace {
  G4Mutex runActionMutex = G4MUTEX_INITIALIZER;
}

Tangle2RunAction::Tangle2RunAction()
{
  if (G4Threading::IsMasterThread()) {
    fpMasterRunAction = this;
  }
  G4AutoLock lock(&runActionMutex);
}

Tangle2RunAction::~Tangle2RunAction()
{
  delete G4AnalysisManager::Instance();
}

void Tangle2RunAction::BeginOfRunAction(const G4Run*)
{
   G4cout
  << "Tangle2RunAction::BeginOfRunAction: Thread: "
  << G4Threading::G4GetThreadId()
  << G4endl;

  if (G4Threading::IsWorkerThread()) {

    Tangle2::nEventsPh = 0;
   
  } else {  // Master thread

    Tangle2::nMasterEventsPh = 0;
  }

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFirstNtupleId(1);
  
  //Create a three-column ntuple
  analysisManager->CreateNtuple("Tangle2", "Energy");
  // 1) total energy released in the crystal ## (double), MeV
  analysisManager->CreateNtupleDColumn("crystal0");
  analysisManager->CreateNtupleDColumn("crystal1");
  analysisManager->CreateNtupleDColumn("crystal2");
  analysisManager->CreateNtupleDColumn("crystal3");
  analysisManager->CreateNtupleDColumn("crystal4");
  analysisManager->CreateNtupleDColumn("crystal5");
  analysisManager->CreateNtupleDColumn("crystal6");
  analysisManager->CreateNtupleDColumn("crystal7");
  analysisManager->CreateNtupleDColumn("crystal8");
  analysisManager->CreateNtupleDColumn("crystal9");
  analysisManager->CreateNtupleDColumn("crystal10");
  analysisManager->CreateNtupleDColumn("crystal11");
  analysisManager->CreateNtupleDColumn("crystal12");
  analysisManager->CreateNtupleDColumn("crystal13");
  analysisManager->CreateNtupleDColumn("crystal14");
  analysisManager->CreateNtupleDColumn("crystal15");
  analysisManager->CreateNtupleDColumn("crystal16");
  analysisManager->CreateNtupleDColumn("crystal17");
  //number of Compton scattering processes in an event
  analysisManager->CreateNtupleIColumn("nb_Compt");
  //momentum direction (unit vector) if 'true QE event'
  //detector A (cryst 0-8)
  analysisManager->CreateNtupleDColumn("AdX");
  analysisManager->CreateNtupleDColumn("AdY");
  analysisManager->CreateNtupleDColumn("AdZ");
  //detector B (cryst 9-17)
  analysisManager->CreateNtupleDColumn("BdX");
  analysisManager->CreateNtupleDColumn("BdY");
  analysisManager->CreateNtupleDColumn("BdZ");
  analysisManager->FinishNtuple();

  analysisManager->OpenFile("Tangle2");

  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

}

namespace {
  G4Mutex mutex = G4MUTEX_INITIALIZER;
}

void Tangle2RunAction::EndOfRunAction(const G4Run* run)
{
  if (G4Threading::IsWorkerThread()) {

    G4cout
    << "Tangle2RunAction::EndOfRunAction: Thread: "
    << G4Threading::G4GetThreadId()
    << ", " << Tangle2::nEventsPh << " 511 keV deposit events"
    << G4endl;
    
    // Always use a lock when writing to a location that is shared by threads
    G4AutoLock lock(&mutex);
    Tangle2::nMasterEventsPh += Tangle2::nEventsPh;

  } else {  // Master thread
    Tangle2::nMasterEventsPh += Tangle2::nEventsPh;
    G4cout
     << "Tangle2RunAction::EndOfRunAction: Master thread: "
    << Tangle2::nMasterEventsPh << " 511 keV deposit events"
    << G4endl;
    
    Tangle2::outFile
    << "#,RunId,nEventsPh"
    << std::endl;
    Tangle2::outFile
    << "#," << run->GetRunID()
    << ',' << Tangle2::nMasterEventsPh
    << std::endl;
  }

  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();

}
