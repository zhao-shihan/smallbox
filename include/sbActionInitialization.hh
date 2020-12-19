#ifndef SB_ACTION_INITIALIZATION_H
#define SB_ACTION_INITIALIZATION_H 1

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization();
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif


