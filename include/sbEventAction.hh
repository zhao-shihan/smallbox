#ifndef SB_EVENT_ACTION_H
#define SB_EVENT_ACTION_H 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "sbRunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

class RunAction;

class EventAction : public G4UserEventAction {
public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

private:
    RunAction* fRunAction;
};

#endif


