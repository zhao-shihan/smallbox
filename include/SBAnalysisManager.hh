#ifndef SB_ANALYSIS_MANAGER
#define SB_ANALYSIS_MANAGER 1

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

#include "SBConfigs.hh"

#if SB_SAVE_TYPE == 0
#include "g4analysis.hh"
#include "G4MTRunManager.hh"

class SBAnalysisManager {
private:
    static G4String fFileName;
    static size_t fEventSN;
    static G4bool fRunHasCompleted;

    const G4int* fpSiPMCount;

public:
    SBAnalysisManager();
    ~SBAnalysisManager() {}

    void Open() const;
    void FillMaxResponse(const std::vector<G4double>& maxResponseList) const;
    void FillMuonGenerateInfo(G4double x, G4double y, G4double phi, G4double theta, G4double energy) const;
    void FillMuonHitInfo(G4double x, G4double y, G4double phi, G4double theta, G4double energy) const;
    void FillPhotonInfo(G4int eventID, G4int SiPMID, G4double time, G4double energy) const;
    void FillWaveForm(G4int eventID, G4int SiPMID, G4double time, G4double response) const;
    void WriteAndClose() const;

    static void SetFileName(const G4String& rootFileName) { fFileName = rootFileName; }
    static const G4String& GetFileName() { return fFileName; }
    void EventComplete() { ++fEventSN; }
    size_t GetEventSN() const { return fEventSN; }
};

#elif SB_SAVE_TYPE == 1
#include <fstream>
#include <vector>
#include "SBMuonProperty.hh"

class SBAnalysisManager {
private:
    static G4String fFileName;
    static std::ofstream* fout;
    static G4bool fIsContinue;
    static G4bool fRunHasCompleted;

    const G4int* fpSiPMCount;

    thread_local static SBMuonProperty fMuonGenerateInfo;
    thread_local static SBMuonProperty fMuonHitInfo;
    thread_local static std::vector<G4double>&& fMaxResponseList;

public:
    SBAnalysisManager();
    ~SBAnalysisManager() {}

    void Open() const;
    void FillMaxResponse(const std::vector<G4double>& maxResponseList);
    void FillMuonGenerateInfo(G4double x, G4double y, G4double phi, G4double theta, G4double energy);
    void FillMuonHitInfo(G4double x, G4double y, G4double phi, G4double theta, G4double energy);
    void EventComplete() const;
    void WriteAndClose() const;

    static void SetFileName(const G4String& csvFileName) { fFileName = csvFileName; }
    static const G4String& GetFileName() { return fFileName; }
    static void IsContinue() { fIsContinue = true; }
};

#endif

#endif

