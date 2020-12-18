#ifndef PHYSICS_LIST_H
#define PHYSICS_LIST_H 1

#include "G4VModularPhysicsList.hh"

class PhysicsList : public G4VModularPhysicsList {
public:
    PhysicsList();
    virtual ~PhysicsList();

    virtual void SetCuts();
};

#endif