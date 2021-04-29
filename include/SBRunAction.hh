#ifndef SB_RUN_ACTION_H
#define SB_RUN_ACTION_H 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "SBAnalysisManager.hh"
#include "SBProgressMonitor.hh"

class SBRunAction : public G4UserRunAction {
private:
    SBAnalysisManager* fAnalysisManager;
    SBProgressMonitor* fProgressMonitor;

    static G4bool fRunHasCompleted;

public:
    SBRunAction();
    virtual ~SBRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
};

#endif

