#include "SBEventAction.hh"

SBEventAction::SBEventAction() :
    G4UserEventAction(),
    fAnalysisManager(new SBAnalysisManager()),
    fProgressMonitor(SBProgressMonitor::Instance()) {}

SBEventAction::~SBEventAction() { delete fAnalysisManager; }

void SBEventAction::BeginOfEventAction(const G4Event*) {}

void SBEventAction::EndOfEventAction(const G4Event*) {
    fAnalysisManager->EventComplete();
    fProgressMonitor->EventComplete();
}

