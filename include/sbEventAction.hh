#ifndef SB_EVENT_ACTION_H
#define SB_EVENT_ACTION_H 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "sbRunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

class sbRunAction;

class sbEventAction : public G4UserEventAction {
public:
    sbEventAction(sbRunAction* runAction);
    virtual ~sbEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

private:
    sbRunAction* fRunAction;
};

#endif


