#include "Tangle2Data.hh"

//G4Mutex Tangle2::outFileMutex = G4MUTEX_INITIALIZER;
//std::ofstream Tangle2::outFile("outFile.csv");

G4int Tangle2::nMasterEventsPh = 0;

// Worker quantities
G4ThreadLocal G4int Tangle2::nEventsPh = 0;
G4ThreadLocal G4double Tangle2::eDepEvent = 0.;
G4ThreadLocal G4double Tangle2::eDepCryst[18] ={0.};
//G4ThreadLocal G4double Tangle2::eDepColl1 =0.;
//G4ThreadLocal G4double Tangle2::eDepColl2 =0.;
G4ThreadLocal G4int Tangle2::nb_Compt[18] ={0};
G4ThreadLocal G4int Tangle2::countA1 = 0;
G4ThreadLocal G4int Tangle2::countA2 = 0;
G4ThreadLocal G4int Tangle2::countB1 = 0;
G4ThreadLocal G4int Tangle2::countB2 = 0;
G4ThreadLocal G4ThreeVector Tangle2::posA_1 = G4ThreeVector();
G4ThreadLocal G4ThreeVector Tangle2::posB_1 = G4ThreeVector();
G4ThreadLocal G4ThreeVector Tangle2::posA_2 = G4ThreeVector();
G4ThreadLocal G4ThreeVector Tangle2::posB_2 = G4ThreeVector();
G4ThreadLocal G4double Tangle2::thetaA = 0;
G4ThreadLocal G4double Tangle2::thetaB = 0;
G4ThreadLocal G4double Tangle2::phiA = 0;
G4ThreadLocal G4double Tangle2::phiB = 0;
G4ThreadLocal G4double Tangle2::dphi = 0;


