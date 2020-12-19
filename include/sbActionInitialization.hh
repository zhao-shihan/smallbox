#ifndef SB_ACTION_INITIALIZATION_H
#define SB_ACTION_INITIALIZATION_H 1

#include "G4VUserActionInitialization.hh"

#include "sbPrimaryGeneratorAction.hh"
#include "sbRunAction.hh"
#include "sbEventAction.hh"
#include "sbSteppingAction.hh"

class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization();
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif


