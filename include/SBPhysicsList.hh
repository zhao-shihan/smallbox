#ifndef SB_PHYSICS_LIST_H
#define SB_PHYSICS_LIST_H 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class SBPhysicsList : public G4VModularPhysicsList {
public:
    SBPhysicsList();
    virtual ~SBPhysicsList();
    virtual void SetCuts();
};

#endif

