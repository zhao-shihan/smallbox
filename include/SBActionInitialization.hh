#ifndef SB_ACTION_INITIALIZATION_H
#define SB_ACTION_INITIALIZATION_H 1

#include "G4VUserActionInitialization.hh"

class SBActionInitialization : public G4VUserActionInitialization {
public:
    SBActionInitialization();
    virtual ~SBActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif


