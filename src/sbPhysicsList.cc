#include "sbPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList() {
    SetVerboseLevel(1);
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());
}

PhysicsList::~PhysicsList() {}

void PhysicsList::SetCuts() {
    G4VUserPhysicsList::SetCuts();
}
