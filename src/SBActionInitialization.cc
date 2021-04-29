#include "SBActionInitialization.hh"
#include "SBPrimaryGeneratorAction.hh"
#include "SBRunAction.hh"
#include "SBEventAction.hh"

SBActionInitialization::SBActionInitialization() :
    G4VUserActionInitialization() {}

SBActionInitialization::~SBActionInitialization() {}

void SBActionInitialization::BuildForMaster() const {
    SetUserAction(new SBRunAction());
}

void SBActionInitialization::Build() const {
    SetUserAction(new SBPrimaryGeneratorAction());
    SetUserAction(new SBRunAction());
    SetUserAction(new SBEventAction());
}

