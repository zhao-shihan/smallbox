#ifndef SB_PHYSICS_LIST_H
#define SB_PHYSICS_LIST_H 1

#include "G4VModularPhysicsList.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

class secOpticalPhysics;
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
    secOpticalPhysics* OpticalPhysics_init(void);
    G4DecayPhysics* DecayPhysics_init(void);
    G4EmStandardPhysics* EmPhysics_init(void);
    G4HadronElasticPhysics* HadronElasticPhysics_init(void);
    G4IonPhysics* IonPhysics_init(void);
    G4StepLimiterPhysics* StepLimiter_init(void);
};

#endif

