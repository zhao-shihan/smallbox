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

sbRunAction::sbRunAction() :
    G4UserRunAction() {
    // Analysis manager
    auto analysisMgr = G4Analysis::ManagerInstance("root");
    G4cout << "G4Analysis manager is using " << analysisMgr->GetType() << G4endl;

    // setting
    analysisMgr->SetNtupleMerging(true);
    analysisMgr->SetVerboseLevel(1);
    analysisMgr->SetFileName(gRootFileName);

    // book histogram and ntuple
#if SB_PROCESS_SIPM_HIT
    // Upper SiPM, SiPM 0, tupleID = 0
    analysisMgr->CreateNtuple("UpperSiPM", "Hits");
    analysisMgr->CreateNtupleDColumn("HitTime");
    analysisMgr->CreateNtupleDColumn("PhotonEnergy");
    analysisMgr->FinishNtuple();

    // Lower SiPM, SiPM 1, tupleID = 1
    analysisMgr->CreateNtuple("LowerSiPM", "Hits");
    analysisMgr->CreateNtupleDColumn("HitTime");
    analysisMgr->CreateNtupleDColumn("PhotonEnergy");
    analysisMgr->FinishNtuple();
#endif
}

sbRunAction::~sbRunAction() {
    delete G4AnalysisManager::Instance();
}

void sbRunAction::BeginOfRunAction(const G4Run*) {
    G4AnalysisManager::Instance()->OpenFile();
}

void sbRunAction::EndOfRunAction(const G4Run*) {
    G4AnalysisManager::Instance()->Write();
    G4AnalysisManager::Instance()->CloseFile();
}

