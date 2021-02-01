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
    analysisMgr->CreateNtuple("Received", "Hits");
    analysisMgr->CreateNtupleDColumn("PhotonEnergy");
    analysisMgr->FinishNtuple();

    /* analysisMgr->CreateNtuple("Incident", "IncidentMuon");
    analysisMgr->CreateNtupleDColumn("MuonEnergy");
    analysisMgr->CreateNtupleDColumn("MuonMomentum");
    analysisMgr->FinishNtuple(); */
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

