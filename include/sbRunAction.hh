#ifndef SB_RUN_ACTION_H
#define SB_RUN_ACTION_H 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "g4analysis.hh"
#include "sbGlobal.hh"

class G4Run;

class sbRunAction : public G4UserRunAction {
public:
    sbRunAction();
    virtual ~sbRunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    G4ToolsAnalysisManager* fAnalysisManager;
};

#endif

