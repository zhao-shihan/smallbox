#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "sbRunAction.hh"
#include "sbPrimaryGeneratorAction.hh"
#include "sbDetectorConstruction.hh"
#include "sbSiPMSD.hh"

sbRunAction::sbRunAction() :
    G4UserRunAction(),
    fAnalysisManager(G4Analysis::ManagerInstance("root")) {
    G4cout << "G4Analysis manager is using " << fAnalysisManager->GetType() << G4endl;

    // setting
    fAnalysisManager->SetNtupleMerging(true);
    fAnalysisManager->SetVerboseLevel(1);
    fAnalysisManager->SetFileName(gRootFileName);
}

sbRunAction::~sbRunAction() {
    delete G4AnalysisManager::Instance();
}

void sbRunAction::BeginOfRunAction(const G4Run*
#if SB_PROCESS_SIPM_HIT 
    run
#endif
) {
#if SB_PROCESS_SCINTILLATOR_HIT
    fAnalysisManager->CreateH1("UpperScintillatorMuonEnergy", "MuonEnergy", 100, 0, 105 * GeV, "GeV");
    fAnalysisManager->CreateH1("LowerScintillatorMuonEnergy", "MuonEnergy", 100, 0, 105 * GeV, "GeV");
    fAnalysisManager->CreateH1("UpperScintillatorMuonZenith", "MuonZenithAngle", 100, 0, M_PI_2, "rad");
    fAnalysisManager->CreateH1("LowerScintillatorMuonZenith", "MuonZenithAngle", 100, 0, M_PI_2, "rad");
#endif
#if SB_PROCESS_SIPM_HIT
    sbSiPMSD::fCurrentNtupleID = -1;
    for (G4int sn = 0; sn < run->GetNumberOfEventToBeProcessed(); ++sn) {
        std::string snStr;
        std::stringstream ss;
        ss << sn;
        ss >> snStr;

        // Upper SiPM photon hit
        fAnalysisManager->CreateNtuple("UpperSiPMOpticalPhotonHits" + snStr, "OpticalPhotonHits");
        fAnalysisManager->CreateNtupleDColumn("Time[ns]");
        fAnalysisManager->CreateNtupleDColumn("PhotonEnergy[eV]");
        fAnalysisManager->FinishNtuple();

        // Lower SiPM photon hit
        fAnalysisManager->CreateNtuple("LowerSiPMOpticalPhotonHits" + snStr, "OpticalPhotonHits");
        fAnalysisManager->CreateNtupleDColumn("Time[ns]");
        fAnalysisManager->CreateNtupleDColumn("PhotonEnergy[eV]");
        fAnalysisManager->FinishNtuple();
    }
#endif
    G4AnalysisManager::Instance()->OpenFile();
}

void sbRunAction::EndOfRunAction(const G4Run*) {
    G4AnalysisManager::Instance()->Write();
    G4AnalysisManager::Instance()->CloseFile();
    
}

