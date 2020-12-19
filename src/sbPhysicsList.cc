#include "sbPhysicsList.hh"

sbPhysicsList::sbPhysicsList() : G4VModularPhysicsList() {
    SetVerboseLevel(1);
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());
}

sbPhysicsList::~sbPhysicsList() {}

void sbPhysicsList::SetCuts() {
    G4VUserPhysicsList::SetCuts();
}
