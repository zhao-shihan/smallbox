#ifndef SB_STEPPING_ACTION_H
#define SB_STEPPING_ACTION_H 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class EventAction;

class G4LogicalVolume;

/// Stepping action class
/// 

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

private:
    EventAction* fEventAction;
};

#endif
