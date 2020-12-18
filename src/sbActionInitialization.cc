#include "sbActionInitialization.hh"
#include "sbPrimaryGeneratorAction.hh"
#include "sbRunAction.hh"
#include "sbEventAction.hh"
#include "sbSteppingAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
    RunAction* runAction = new RunAction;
    SetUserAction(runAction);
}

void ActionInitialization::Build() const {
    SetUserAction(new PrimaryGeneratorAction);

    RunAction* runAction = new RunAction;
    SetUserAction(runAction);

    EventAction* eventAction = new EventAction(runAction);
    SetUserAction(eventAction);

    SteppingAction* steppingAction = new SteppingAction(eventAction);
    SetUserAction(steppingAction);
}

