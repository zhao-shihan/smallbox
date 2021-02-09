#ifndef SB_STEPPING_ACTION_H
#define SB_STEPPING_ACTION_H 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class sbEventAction;

class G4LogicalVolume;

/// Stepping action class
/// 

class sbSteppingAction : public G4UserSteppingAction {
public:
    sbSteppingAction(sbEventAction* eventAction);
    virtual ~sbSteppingAction();

    virtual void UserSteppingAction(const G4Step*);

private:
    sbEventAction* fEventAction;
};

#endif
