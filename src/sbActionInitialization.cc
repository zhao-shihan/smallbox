#include "sbActionInitialization.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
    sbRunAction* runAction = new sbRunAction();
    SetUserAction(runAction);
}

void ActionInitialization::Build() const {
    SetUserAction(new sbPrimaryGeneratorAction);

    sbRunAction* runAction = new sbRunAction();
    SetUserAction(runAction);

    sbEventAction* eventAction = new sbEventAction(runAction);
    SetUserAction(eventAction);

    sbSteppingAction* steppingAction = new sbSteppingAction(eventAction);
    SetUserAction(steppingAction);
}

