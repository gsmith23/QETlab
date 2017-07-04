#include "Tangle2Data.hh"

G4Mutex Tangle2::outFileMutex = G4MUTEX_INITIALIZER;
std::ofstream Tangle2::outFile("outFile.csv");

G4int Tangle2::nMasterEventsPh = 0;

// Worker quantities
G4ThreadLocal G4int Tangle2::nEventsPh = 0;
G4ThreadLocal G4double Tangle2::eDepEvent = 0.;
G4ThreadLocal G4double Tangle2::eDepCryst[18] ={0.};
G4ThreadLocal G4int Tangle2::nb_Compt = 0;
G4ThreadLocal G4ThreeVector Tangle2::posA = G4ThreeVector();
G4ThreadLocal G4ThreeVector Tangle2::posB = G4ThreeVector();
G4ThreadLocal G4double Tangle2::thetaA = 0;
G4ThreadLocal G4double Tangle2::thetaB = 0;
G4ThreadLocal G4double Tangle2::phiA = 0;
G4ThreadLocal G4double Tangle2::phiB = 0;
G4ThreadLocal G4double Tangle2::dphi = 0;
