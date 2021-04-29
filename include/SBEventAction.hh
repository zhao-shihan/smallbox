#ifndef MuTQ_EVENT_ACTION_H
#define MuTQ_EVENT_ACTION_H 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "SBRunAction.hh"
#include "SBAnalysisManager.hh"
#include "SBProgressMonitor.hh"

class SBEventAction : public G4UserEventAction {
private:
    SBAnalysisManager* fAnalysisManager;
    SBProgressMonitor* fProgressMonitor;

public:
    SBEventAction();
    virtual ~SBEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
};

#endif


