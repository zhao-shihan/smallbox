#ifndef SB_SIPM_SD_H
#define SB_SIPM_SD_H 1

#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

#include "SBSiPMHit.hh"
#include "SBAnalysisManager.hh"
#include "SBDetectorConstruction.hh"

class SBDetectorConstruction;

class SBSiPMSD : public G4VSensitiveDetector {
private:
    const G4int* fpSiPMCount;
    std::vector<SBSiPMHitsCollection*> fSiPMPhotonHCList;

    SBAnalysisManager* fAnalysisManager;
    const SBDetectorConstruction* fDetectorConstruction;

public:
    SBSiPMSD(const G4String& SiPMSDName);
    virtual ~SBSiPMSD();

    virtual void Initialize(G4HCofThisEvent* eventHitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory*);
    virtual void EndOfEvent(G4HCofThisEvent*);

private:
    std::vector<std::pair<G4double, G4double>>
        ConstructSiPMPhotoelectricResponse(const std::vector<SBSiPMHit*>& hitList) const;
    inline G4double SiPMSinglePhotoelectricResponse(const G4double& elapsedTimeAfterHit) const;

    static bool compareHit(SBSiPMHit* lhs, SBSiPMHit* rhs) { return lhs->GetTime() < rhs->GetTime(); }
    static bool compareResponse(const std::pair<G4double, G4double> lhs, const std::pair<G4double, G4double> rhs) {
        return lhs.second < rhs.second;
    }
};

inline G4double SBSiPMSD::SiPMSinglePhotoelectricResponse(const G4double& elapsedTimeAfterHit) const {
    constexpr auto A = 1.0;
    constexpr auto tau = 1.0 * ns;
    auto&& u = log(elapsedTimeAfterHit / tau);
    return A * exp(-u * u);
}

#endif

