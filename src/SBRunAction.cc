#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "SBRunAction.hh"
#include "SBPrimaryGeneratorAction.hh"
#include "SBDetectorConstruction.hh"
#include "SBConfigs.hh"

SBRunAction::SBRunAction() :
    G4UserRunAction(),
    fAnalysisManager(new SBAnalysisManager()),
    fProgressMonitor(SBProgressMonitor::Instance()) {}

SBRunAction::~SBRunAction() {
    delete fAnalysisManager;
}

void SBRunAction::BeginOfRunAction(const G4Run*) {
#if SB_SAVE_ANYTHING
    fAnalysisManager->Open();
#endif
    fProgressMonitor->RunStart();
}

void SBRunAction::EndOfRunAction(const G4Run*) {
#if SB_SAVE_ANYTHING
    fAnalysisManager->WriteAndClose();
#endif
    fProgressMonitor->RunComplete();
}

