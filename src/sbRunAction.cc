#include "sbRunAction.hh"
#include "sbPrimaryGeneratorAction.hh"
#include "sbDetectorConstruction.hh"
// #include "Run.hh"

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

void RunAction::EndOfRunAction(const G4Run* run) {
    G4int nofEvents = run->GetNumberOfEvent();//获取本次Run中模拟了多少个event
    if (nofEvents == 0) return;//如果模拟了0个，则返回

    G4cout
        << G4endl
        << " The run consists of " << nofEvents << " events"
        << G4endl
        << "------------------------------------------------------------"
        << G4endl;
}

