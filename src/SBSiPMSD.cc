#include <sstream>
#include <algorithm>

#include "G4OpticalPhoton.hh"
#include "G4MTRunManager.hh"

#include "SBSiPMSD.hh"
#include "SBRunAction.hh"
#include "SBDetectorConstruction.hh"
#include "SBConfigs.hh"

SBSiPMSD::SBSiPMSD(const G4String& SiPMSDName) :
    G4VSensitiveDetector(SiPMSDName),
    fpSiPMCount(&static_cast<const SBDetectorConstruction*>(
        G4MTRunManager::GetMasterRunManager()->GetUserDetectorConstruction())->fSiPMCount),
    fSiPMPhotonHCList(*fpSiPMCount, nullptr),
    fAnalysisManager(new SBAnalysisManager()),
    fDetectorConstruction(static_cast<const SBDetectorConstruction*>(
        G4MTRunManager::GetMasterRunManager()->GetUserDetectorConstruction())) {
    std::stringstream ss;
    std::string name;
    for (G4int SiPMID = 0; SiPMID < *fpSiPMCount; ++SiPMID) {
        ss << "SiPM_" << SiPMID << "_hits_collection";
        ss >> name;
        collectionName.push_back(name);
        ss.clear();
        name.clear();
    }
}

SBSiPMSD::~SBSiPMSD() {
    delete fAnalysisManager;
}

void SBSiPMSD::Initialize(G4HCofThisEvent* hitCollectionOfThisEvent) {
    for (G4int SiPMID = 0; SiPMID < *fpSiPMCount; ++SiPMID) {
        fSiPMPhotonHCList[SiPMID] = new SBSiPMHitsCollection(SensitiveDetectorName, collectionName[SiPMID]);
        hitCollectionOfThisEvent->AddHitsCollection(GetCollectionID(SiPMID), fSiPMPhotonHCList[SiPMID]);
    }
}

G4bool SBSiPMSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    if (step->GetTrack()->GetParticleDefinition() != G4OpticalPhoton::Definition()) {
        return false;
    }
    auto hit = new SBSiPMHit(
        step->GetPreStepPoint()->GetGlobalTime(),
        step->GetPreStepPoint()->GetTotalEnergy()
    );
    fSiPMPhotonHCList[step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo()
        / fDetectorConstruction->GetDetectorPartCount()]->insert(hit);
    return true;
}

void SBSiPMSD::EndOfEvent(G4HCofThisEvent*) {
#if SB_SAVE_ANYTHING
    bool noSiPMWasHit = true;
    for (const auto& SiPMPhotonHC : fSiPMPhotonHCList) {
        noSiPMWasHit &= (SiPMPhotonHC->entries() == 0);
    }
    if (noSiPMWasHit) {
#if SB_SAVE_SIPM_MAX_RESPONSE
        fAnalysisManager->FillMaxResponse(std::vector<G4double>(*fpSiPMCount, 0.0));
#endif
        return;
    }
#if SB_SAVE_PHOTON_HIT_INFO || SB_SAVE_SIPM_RESPONSE_WAVEFORM
    const G4int& eventSN = fAnalysisManager->GetEventSN();
#endif
#if SB_SAVE_SIPM_MAX_RESPONSE
    std::vector<G4double> maxResponse(*fpSiPMCount);
#endif
    for (G4int SiPMID = 0; SiPMID < *fpSiPMCount; ++SiPMID) {
#if SB_SAVE_SIPM_RESPONSE_WAVEFORM || SB_SAVE_SIPM_MAX_RESPONSE
        std::vector<SBSiPMHit*> hitList(fSiPMPhotonHCList[SiPMID]->entries());
#endif
        for (size_t photonID = 0; photonID < fSiPMPhotonHCList[SiPMID]->entries(); ++photonID) {
            auto hit = static_cast<SBSiPMHit*>(fSiPMPhotonHCList[SiPMID]->GetHit(photonID));
#if SB_SAVE_PHOTON_HIT_INFO
            fAnalysisManager->FillPhotonInfo(eventSN, SiPMID, hit->GetTime(), hit->GetEnergy());
#endif
#if SB_SAVE_SIPM_RESPONSE_WAVEFORM || SB_SAVE_SIPM_MAX_RESPONSE
            hitList[photonID] = hit;
#endif
        }
#if SB_SAVE_SIPM_RESPONSE_WAVEFORM || SB_SAVE_SIPM_MAX_RESPONSE
        std::sort(hitList.begin(), hitList.end(), this->compareHit);
        auto&& response = ConstructSiPMPhotoelectricResponse(hitList);
#endif
#if SB_SAVE_SIPM_RESPONSE_WAVEFORM
        for (const auto& aResponse : response) {
            fAnalysisManager->FillWaveForm(eventSN, SiPMID, aResponse.first, aResponse.second);
        }
#endif
#if SB_SAVE_SIPM_MAX_RESPONSE
        maxResponse[SiPMID] = std::max_element(response.begin(), response.end(), this->compareResponse)->second;
#endif
    }
#if SB_SAVE_SIPM_MAX_RESPONSE
    fAnalysisManager->FillMaxResponse(maxResponse);
#endif
#endif
}

std::vector<std::pair<G4double, G4double>>
SBSiPMSD::ConstructSiPMPhotoelectricResponse(const std::vector<SBSiPMHit*>& hitList) const {
    constexpr G4double timeRes = 0.1 * ns;
    constexpr G4double maxTime = 10 * us;
    constexpr G4double timeWindow = 5 * ns;
    std::vector<std::pair<G4double, G4double>> response(size_t(maxTime / timeRes) + 1, { 0.0, 0.0 });
    // set time.
    for (auto it = response.begin() + 1; it != response.end(); ++it) {
        it->first = (it - 1)->first + timeRes;
    }
    // construct photoelectric response.
    auto windowBeginHit = hitList.begin();
    auto windowEndHit = hitList.begin();
    for (auto& aResponse : response) {
        for (;
            windowBeginHit != hitList.end() &&
            (*windowBeginHit)->GetTime() < aResponse.first - timeWindow;
            ++windowBeginHit);
        for (;
            windowEndHit != hitList.end() &&
            (*windowEndHit)->GetTime() < aResponse.first;
            ++windowEndHit);
        for (auto hit = windowBeginHit; hit != windowEndHit; ++hit) {
            aResponse.second += SiPMSinglePhotoelectricResponse(aResponse.first - (*hit)->GetTime());
        }
    }
    return response;
}

