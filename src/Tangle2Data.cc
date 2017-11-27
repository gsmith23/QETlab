#include "Tangle2Data.hh"

// change to false to run with 
// back to back photons
G4bool Tangle2::positrons = false;
G4bool Tangle2::fixedAxis = false;
G4bool Tangle2::perpPol   = false;

// For runs with multi-threading
G4int Tangle2::nMasterEventsPh = 0;
G4int Tangle2::nMasterEvents = 0;

// Worker quantities
G4ThreadLocal G4int Tangle2::nEvents = 0;
G4ThreadLocal G4int Tangle2::nEventsPh = 0;
G4ThreadLocal G4double Tangle2::eDepEvent = 0.;
G4ThreadLocal G4double Tangle2::eDepCryst[18] ={0.};
G4ThreadLocal G4double Tangle2::eDepColl1 =0.;
G4ThreadLocal G4double Tangle2::eDepColl2 =0.;
G4ThreadLocal G4int Tangle2::nb_Compt[18] ={0};
G4ThreadLocal G4ThreeVector Tangle2::posA_1 = G4ThreeVector();
G4ThreadLocal G4ThreeVector Tangle2::posB_1 = G4ThreeVector();
G4ThreadLocal G4ThreeVector Tangle2::posA_2 = G4ThreeVector();
G4ThreadLocal G4ThreeVector Tangle2::posB_2 = G4ThreeVector();
G4ThreadLocal G4double Tangle2::thetaA = 0;
G4ThreadLocal G4double Tangle2::thetaB = 0;
G4ThreadLocal G4double Tangle2::phiA = 0;
G4ThreadLocal G4double Tangle2::phiB = 0;
G4ThreadLocal G4double Tangle2::dphi = 0;
G4ThreadLocal G4double Tangle2::thetaA2 = 0;
G4ThreadLocal G4double Tangle2::thetaB2 = 0;
G4ThreadLocal G4double Tangle2::phiA2 = 0;
G4ThreadLocal G4double Tangle2::phiB2 = 0;
