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
#include "sbConfigs.hh"

sbRunAction::sbRunAction() :
    G4UserRunAction(),
    fAnalysisManager(nullptr) {
    if (gRunningInBatch) {
        fAnalysisManager = G4Analysis::ManagerInstance("root");
        G4cout << "G4Analysis manager is using " << fAnalysisManager->GetType() << '.' << G4endl;

        // setting
        fAnalysisManager->SetNtupleMerging(true);
        fAnalysisManager->SetVerboseLevel(1);
        fAnalysisManager->SetFileName(gRootFileName);
    } else {
        G4cout << "Running in interactive mode, G4Analysis manager is disabled." << G4endl;
    }
}

sbRunAction::~sbRunAction() {
    if (gRunningInBatch) {
        delete G4AnalysisManager::Instance();
    }
}

void sbRunAction::BeginOfRunAction(const G4Run* run) {
    if (gRunningInBatch) {
        CreateTreeAndHistrogram(run->GetNumberOfEventToBeProcessed());
        G4AnalysisManager::Instance()->OpenFile();
    }
}

void sbRunAction::EndOfRunAction(const G4Run*) {
    if (gRunningInBatch) {
        G4AnalysisManager::Instance()->Write();
        G4AnalysisManager::Instance()->CloseFile();
    }
}

void sbRunAction::CreateTreeAndHistrogram(G4int numberOfEvent) const {
#if SB_PROCESS_SCINTILLATOR_HIT
    fAnalysisManager->CreateH1("UpperScintillatorMuonEnergy", "MuonEnergy", 100, 0, 105 * GeV, "GeV");
    fAnalysisManager->CreateH1("LowerScintillatorMuonEnergy", "MuonEnergy", 100, 0, 105 * GeV, "GeV");
    fAnalysisManager->CreateH1("UpperScintillatorMuonZenith", "MuonZenithAngle", 100, 0, M_PI_2, "rad");
    fAnalysisManager->CreateH1("LowerScintillatorMuonZenith", "MuonZenithAngle", 100, 0, M_PI_2, "rad");
#endif
#if SB_PROCESS_SIPM_HIT
    sbSiPMSD::fHitEventCount = -1;
    std::string snStr;
    std::stringstream ss;
    for (G4int sn = 0; sn < numberOfEvent; ++sn) {
        ss.clear();
        ss << sn;
        ss >> snStr;

        // Upper SiPM photon hit
        fAnalysisManager->CreateNtuple("UpperSiPMOpticalPhotonHits" + snStr, "OpticalPhotonHits");
        fAnalysisManager->CreateNtupleDColumn("HitTime[ns]");
        fAnalysisManager->CreateNtupleDColumn("PhotonEnergy[eV]");
        fAnalysisManager->FinishNtuple();

        // Lower SiPM photon hit
        fAnalysisManager->CreateNtuple("LowerSiPMOpticalPhotonHits" + snStr, "OpticalPhotonHits");
        fAnalysisManager->CreateNtupleDColumn("HitTime[ns]");
        fAnalysisManager->CreateNtupleDColumn("PhotonEnergy[eV]");
        fAnalysisManager->FinishNtuple();

        // Photoelectric response
        fAnalysisManager->CreateNtuple("SiPMPhotoelectricResponse" + snStr, "PhotoelectricResponse");
        fAnalysisManager->CreateNtupleDColumn("Time[ns]");
        fAnalysisManager->CreateNtupleDColumn("UpperPhotoelectricResponse[a.u.]");
        fAnalysisManager->CreateNtupleDColumn("LowerPhotoelectricResponse[a.u.]");
        fAnalysisManager->FinishNtuple();
    }
#endif
}

