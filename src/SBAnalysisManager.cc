#include "G4MTRunManager.hh"
#include "G4Run.hh"

#include "SBAnalysisManager.hh"
#include "SBDetectorConstruction.hh"

G4Mutex mutex_SBAnalysisManager;

G4String SBAnalysisManager::fFileName = "unnamed";
G4bool SBAnalysisManager::fRunHasCompleted = false;

#if SB_SAVE_TYPE == 0

constexpr G4int profileSampleNum = 10000;
constexpr G4double profileMaxTime = 10 * us;
constexpr G4double profileMaxResponse = 10000;

size_t SBAnalysisManager::fEventSN = 0;

SBAnalysisManager::SBAnalysisManager() :
    fpSiPMCount(&static_cast<const SBDetectorConstruction*>(
        G4MTRunManager::GetMasterRunManager()->GetUserDetectorConstruction())->fSiPMCount) {}

void SBAnalysisManager::Open() const {
#if SB_SAVE_ANYTHING
    auto g4AnalysisManager = G4Analysis::ManagerInstance("root");
    g4AnalysisManager->SetNtupleMerging(true);
    std::stringstream ss;
#endif
#if SB_SAVE_SIPM_MAX_RESPONSE
    g4AnalysisManager->SetFirstNtupleId(-3);
    // -3
    g4AnalysisManager->CreateNtuple("SiPMMaxPhotoelectricResponse", "MaxResponse");
    for (G4int SiPMID = 0; SiPMID < *fpSiPMCount; ++SiPMID) {
        std::string columnName;
        ss << "SiPM" << SiPMID;
        ss >> columnName;
        ss.clear();
        g4AnalysisManager->CreateNtupleDColumn(columnName);
    }
    g4AnalysisManager->FinishNtuple();
#else
    g4AnalysisManager->SetFirstNtupleId(-2);
#endif
#if SB_SAVE_ANYTHING
    // -2
    g4AnalysisManager->CreateNtuple("MuonGenerateInfo", "info");
    g4AnalysisManager->CreateNtupleDColumn("PositionX[cm]");
    g4AnalysisManager->CreateNtupleDColumn("PositionY[cm]");
    g4AnalysisManager->CreateNtupleDColumn("AzimuthAngle[rad]");
    g4AnalysisManager->CreateNtupleDColumn("ZenithAngle[rad]");
    g4AnalysisManager->CreateNtupleDColumn("Energy[MeV]");
    g4AnalysisManager->FinishNtuple();
    // -1
    g4AnalysisManager->CreateNtuple("MuonHitInfo", "info");
    g4AnalysisManager->CreateNtupleDColumn("PositionX[cm]");
    g4AnalysisManager->CreateNtupleDColumn("PositionY[cm]");
    g4AnalysisManager->CreateNtupleDColumn("AzimuthAngle[rad]");
    g4AnalysisManager->CreateNtupleDColumn("ZenithAngle[rad]");
    g4AnalysisManager->CreateNtupleDColumn("Energy[MeV]");
    g4AnalysisManager->FinishNtuple();
#endif
#if SB_SAVE_PHOTON_HIT_INFO || SB_SAVE_SIPM_RESPONSE_WAVEFORM
    std::vector<G4String> SiPMNtupleNameList(*fpSiPMCount);
    std::vector<G4String> SiPMP1NameList(*fpSiPMCount);
    for (G4int SiPMID = 0; SiPMID < *fpSiPMCount; ++SiPMID) {
#if SB_SAVE_PHOTON_HIT_INFO
        ss << "SiPM" << SiPMID << "PhotonHitInfo";
        ss >> SiPMNtupleNameList[SiPMID];
        ss.clear();
#endif
#if SB_SAVE_SIPM_RESPONSE_WAVEFORM
        ss << "SiPM" << SiPMID << "PhotoelectricResponse";
        ss >> SiPMP1NameList[SiPMID];
        ss.clear();
#endif
    }
    for (G4int eventSN = 0;
        eventSN < G4MTRunManager::GetMasterRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed(); ++eventSN) {
        std::string eventSNString;
        ss << eventSN;
        ss >> eventSNString;
        ss.clear();
#if SB_SAVE_PHOTON_HIT_INFO
        for (const auto& ntupleName : SiPMNtupleNameList) {
            g4AnalysisManager->CreateNtuple(ntupleName + eventSNString, "PhotoelectricResponse");
            g4AnalysisManager->CreateNtupleDColumn("Time[ns]");
            g4AnalysisManager->CreateNtupleDColumn("Energy[eV]");
            g4AnalysisManager->FinishNtuple();
        }
#endif
#if SB_SAVE_SIPM_RESPONSE_WAVEFORM
        for (const auto& P1Name : SiPMP1NameList) {
            g4AnalysisManager->CreateP1(
                P1Name + eventSNString, "PhotoelectricResponse",
                profileSampleNum,
                0.0, profileMaxTime,
                0.0, profileMaxResponse,
                "ns", "none"
            );
        }
#endif
    }
#endif

    G4AnalysisManager::Instance()->OpenFile(fFileName + ".root");
}

void SBAnalysisManager::WriteAndClose() const {
    G4AnalysisManager::Instance()->Write();
    G4AnalysisManager::Instance()->CloseFile();
    auto run = G4MTRunManager::GetMasterRunManager()->GetCurrentRun();
    if (run->GetNumberOfEvent() == run->GetNumberOfEventToBeProcessed() && !fRunHasCompleted) {
        delete G4AnalysisManager::Instance();
        fRunHasCompleted = true;
    }
}

void SBAnalysisManager::FillMaxResponse(const std::vector<G4double>& maxResponseList) const {
    auto g4AnalysisManager = G4AnalysisManager::Instance();
    for (G4int SiPMID = 0; SiPMID < *fpSiPMCount; ++SiPMID) {
        g4AnalysisManager->FillNtupleDColumn(-3, SiPMID, maxResponseList[SiPMID]);
    }
    g4AnalysisManager->AddNtupleRow(-3);
}

void SBAnalysisManager::FillMuonGenerateInfo
(G4double x, G4double y, G4double phi, G4double theta, G4double energy) const {
    auto g4AnalysisManager = G4AnalysisManager::Instance();
    g4AnalysisManager->FillNtupleDColumn(-2, 0, x / cm);
    g4AnalysisManager->FillNtupleDColumn(-2, 1, y / cm);
    g4AnalysisManager->FillNtupleDColumn(-2, 2, phi / rad);
    g4AnalysisManager->FillNtupleDColumn(-2, 3, theta / rad);
    g4AnalysisManager->FillNtupleDColumn(-2, 4, energy / MeV);
    g4AnalysisManager->AddNtupleRow(-2);
}

void SBAnalysisManager::FillMuonHitInfo
(G4double x, G4double y, G4double phi, G4double theta, G4double energy) const {
    auto g4AnalysisManager = G4AnalysisManager::Instance();
    g4AnalysisManager->FillNtupleDColumn(-1, 0, x / cm);
    g4AnalysisManager->FillNtupleDColumn(-1, 1, y / cm);
    g4AnalysisManager->FillNtupleDColumn(-1, 2, phi / rad);
    g4AnalysisManager->FillNtupleDColumn(-1, 3, theta / rad);
    g4AnalysisManager->FillNtupleDColumn(-1, 4, energy / MeV);
    g4AnalysisManager->AddNtupleRow(-1);
}

void  SBAnalysisManager::FillPhotonInfo(G4int eventSN, G4int SiPMID, G4double time, G4double energy) const {
    auto g4AnalysisManager = G4AnalysisManager::Instance();
    auto&& ntupleID = eventSN * *fpSiPMCount + SiPMID;
    g4AnalysisManager->FillNtupleDColumn(ntupleID, 0, time / ns);
    g4AnalysisManager->FillNtupleDColumn(ntupleID, 1, energy / eV);
    g4AnalysisManager->AddNtupleRow(ntupleID);
}

void SBAnalysisManager::FillWaveForm
(G4int eventSN, G4int SiPMID, G4double time, G4double response) const {
    G4AnalysisManager::Instance()->FillP1(eventSN * *fpSiPMCount + SiPMID, time, response);
}

#elif SB_SAVE_TYPE == 1

std::ofstream* SBAnalysisManager::fout = nullptr;
G4bool SBAnalysisManager::fIsContinue = false;

thread_local SBMuonProperty SBAnalysisManager::fMuonGenerateInfo;
thread_local SBMuonProperty SBAnalysisManager::fMuonHitInfo;
thread_local std::vector<G4double>&& SBAnalysisManager::fMaxResponseList = std::vector<G4double>();

SBAnalysisManager::SBAnalysisManager() :
    fpSiPMCount(&static_cast<const SBDetectorConstruction*>(
        G4MTRunManager::GetMasterRunManager()->GetUserDetectorConstruction())->fSiPMCount) {}

void SBAnalysisManager::Open() const {
    if (fIsContinue) {
        fout = new std::ofstream(fFileName + ".csv", std::ios::app);
    } else {
        fout = new std::ofstream(fFileName + ".csv");
        *fout <<
            "GenerateX/cm,GenerateY/cm,GeneratePhi/rad,GenerateTheta/rad,GenerateEnergy/MeV,"
            "HitX/cm,HitY/cm,HitPhi/rad,HitTheta/rad,HitEnergy/MeV";
        for (G4int SiPMID = 0; SiPMID < *fpSiPMCount; ++SiPMID) { *fout << ",SiPM" << SiPMID; }
        *fout << std::endl;
    }
}

void SBAnalysisManager::FillMaxResponse(const std::vector<G4double>& maxResponseList) {
    fMaxResponseList = maxResponseList;
}

void SBAnalysisManager::FillMuonGenerateInfo(G4double x, G4double y, G4double phi, G4double theta, G4double energy) {
    fMuonGenerateInfo.X() = x;
    fMuonGenerateInfo.Y() = y;
    fMuonGenerateInfo.Phi() = phi;
    fMuonGenerateInfo.Theta() = theta;
    fMuonGenerateInfo.Energy() = energy;
}

void SBAnalysisManager::FillMuonHitInfo(G4double x, G4double y, G4double phi, G4double theta, G4double energy) {
    fMuonHitInfo.X() = x;
    fMuonHitInfo.Y() = y;
    fMuonHitInfo.Phi() = phi;
    fMuonHitInfo.Theta() = theta;
    fMuonHitInfo.Energy() = energy;
}

void SBAnalysisManager::EventComplete() const {
    mutex_SBAnalysisManager.lock();
    auto WriteMuonInfo = [this](const SBMuonProperty& muonInfo)->void {
        *fout << muonInfo.GetX() / cm << ',' << muonInfo.GetY() / cm << ','
            << muonInfo.GetPhi() / rad << ',' << muonInfo.GetTheta() / rad << ',' << muonInfo.GetEnergy() / MeV;
    };
    WriteMuonInfo(fMuonGenerateInfo); *fout << ',';
    WriteMuonInfo(fMuonHitInfo);
    for (const auto& maxResponse : fMaxResponseList) { *fout << ',' << maxResponse; }
    *fout << std::endl;
    mutex_SBAnalysisManager.unlock();
}

void SBAnalysisManager::WriteAndClose() const {
    auto run = G4MTRunManager::GetMasterRunManager()->GetCurrentRun();
    if (run->GetNumberOfEvent() == run->GetNumberOfEventToBeProcessed() && !fRunHasCompleted) {
        fout->close();
        delete fout;
        fRunHasCompleted = true;
    }
}

#endif

