#include "sbRunAction.hh"
#include "sbPrimaryGeneratorAction.hh"
#include "sbDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction() : G4UserRunAction() {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {}

void RunAction::EndOfRunAction(const G4Run* /* run */) {
    /* G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) {
        return;
    }

    G4cout
        << G4endl
        << " The run consists of " << nofEvents << " events"
        << G4endl
        << "------------------------------------------------------------"
        << G4endl; */
}

