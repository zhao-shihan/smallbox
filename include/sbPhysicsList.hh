#ifndef SB_PHYSICS_LIST_H
#define SB_PHYSICS_LIST_H 1

#include "G4VModularPhysicsList.hh"

#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4SpinDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "sbSwitch.hh"

class G4OpticalPhysics;
class G4DecayPhysics;
class G4EmStandardPhysics;
class G4HadronElasticPhysics;
class G4IonPhysics;
class G4StepLimiterPhysics;

class sbPhysicsList : public G4VModularPhysicsList {
public:
    sbPhysicsList();
    virtual ~sbPhysicsList();
    virtual void SetCuts();

private:
    G4OpticalPhysics* OpticalPhysics_init(void);
};

#endif

