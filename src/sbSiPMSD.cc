#include <sstream>
#include <mutex>

#include "sbSiPMSD.hh"
#include "sbRunAction.hh"
#include "sbDetectorConstruction.hh"

G4int sbSiPMSD::fCurrentNtupleID = -1;

sbSiPMSD::sbSiPMSD(const G4String& SiPMSDName) :
    G4VSensitiveDetector(SiPMSDName),
    fSiPMPhotonHC(nullptr, nullptr),
    fAnalysisManager(G4AnalysisManager::Instance()) {
    collectionName.push_back("upper_optical_photon_hits_collection");
    collectionName.push_back("lower_optical_photon_hits_collection");
}

sbSiPMSD::~sbSiPMSD() {}

void sbSiPMSD::Initialize(G4HCofThisEvent* hitCollectionOfThisEvent) {
    fSiPMPhotonHC.first = new sbSiPMHitsCollection(SensitiveDetectorName, collectionName[0]);
    hitCollectionOfThisEvent->AddHitsCollection(GetCollectionID(0), fSiPMPhotonHC.first);
    fSiPMPhotonHC.second = new sbSiPMHitsCollection(SensitiveDetectorName, collectionName[1]);
    hitCollectionOfThisEvent->AddHitsCollection(GetCollectionID(1), fSiPMPhotonHC.second);
}

G4bool sbSiPMSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    auto presentParticle = step->GetTrack()->GetParticleDefinition();
    if (presentParticle != G4OpticalPhoton::Definition()) {
        return false;
    }
    // Present step point.
    auto preStepPoint = step->GetPreStepPoint();
    // A new hit.
    auto hit = new sbSiPMHit();
    hit->SetTime(preStepPoint->GetGlobalTime());
    hit->SetEnergy(preStepPoint->GetTotalEnergy());
    hit->SetPostion(preStepPoint->GetPosition());
    if (preStepPoint->GetPhysicalVolume() ==
        sbDetectorConstruction::GetsbDCInstance()->GetPhysicalSiPMs().first) {
        fSiPMPhotonHC.first->insert(hit);
    } else if (preStepPoint->GetPhysicalVolume() ==
        sbDetectorConstruction::GetsbDCInstance()->GetPhysicalSiPMs().second) {
        fSiPMPhotonHC.second->insert(hit);
    } else {
        G4ExceptionDescription exceptout;
        exceptout << "The SiPM physical volume not found." << G4endl;
        exceptout << "Maybe you have changed the detector construction?" << G4endl;
        G4Exception(
            "sbSiPMHit::sbSiPMHit(G4VPhysicalVolume* physicalSiPM)",
            "SiPMNotFound",
            FatalException,
            exceptout
        );
        return false;
    }
    return true;
}

std::mutex mtx;

void sbSiPMSD::EndOfEvent(G4HCofThisEvent*) {
    if (!fSiPMPhotonHC.first || !fSiPMPhotonHC.second) {
        G4ExceptionDescription exceptout;
        exceptout << "SiPM optical photon hits collection is null." << G4endl;
        exceptout << "Maybe it was unexpectedly deleted?" << G4endl;
        G4Exception(
            "sbSiPMSD::EndOfEvent(G4HCofThisEvent*)",
            "HCNotFound",
            JustWarning,
            exceptout
        );
        return;
    }
    G4bool emptyUpperHC = fSiPMPhotonHC.first->entries() == 0;
    G4bool emptyLowerHC = fSiPMPhotonHC.second->entries() == 0;
    if (emptyUpperHC && emptyLowerHC) {
        return;
    }

    // Prevent multiple threads from getting the same ntupleID.
    mtx.lock();
    fCurrentNtupleID += 3;
    G4int localCurrentNtupleID = fCurrentNtupleID;
    mtx.unlock();

    G4int upperSiPMHitNtupleID = localCurrentNtupleID - 2;
    G4int lowerSiPMHitNtupleID = localCurrentNtupleID - 1;
    G4int SiPMPhotoelectricResponseNtupleID = localCurrentNtupleID;

    G4cout << "sbSiPMSD::EndOfEvent is filling ntuple "
        << upperSiPMHitNtupleID << " & "
        << lowerSiPMHitNtupleID << " & "
        << SiPMPhotoelectricResponseNtupleID << " ... ";

    G4double upperFirstHitTime = 0.0;
    G4double upperHitTimeAvg = 0.0;
    if (!emptyUpperHC) {
        upperFirstHitTime = static_cast<sbSiPMHit*>(fSiPMPhotonHC.first->GetHit(0))->GetTime() / ns;
        upperHitTimeAvg = 0.0;
        // Fill upper hit ntuple, need units.
        // time in ns, energy in eV.
        for (size_t i = 0; i < fSiPMPhotonHC.first->entries(); ++i) {
            auto hit = static_cast<sbSiPMHit*>(fSiPMPhotonHC.first->GetHit(i));
            fAnalysisManager->FillNtupleDColumn(upperSiPMHitNtupleID, 0, hit->GetTime() / ns);
            fAnalysisManager->FillNtupleDColumn(upperSiPMHitNtupleID, 1, hit->GetEnergy() / eV);
            fAnalysisManager->AddNtupleRow(upperSiPMHitNtupleID);
            upperHitTimeAvg += hit->GetTime();
        }
        upperHitTimeAvg /= ns * fSiPMPhotonHC.first->entries();
    }

    G4double lowerFirstHitTime = 0.0;
    G4double lowerHitTimeAvg = 0.0;
    if (!emptyLowerHC) {
        lowerFirstHitTime = static_cast<sbSiPMHit*>(fSiPMPhotonHC.second->GetHit(0))->GetTime() / ns;
        lowerHitTimeAvg = 0.0;
        // Fill lower hit ntuple.
        for (size_t i = 0; i < fSiPMPhotonHC.second->entries(); ++i) {
            auto hit = static_cast<sbSiPMHit*>(fSiPMPhotonHC.second->GetHit(i));
            fAnalysisManager->FillNtupleDColumn(lowerSiPMHitNtupleID, 0, hit->GetTime() / ns);
            fAnalysisManager->FillNtupleDColumn(lowerSiPMHitNtupleID, 1, hit->GetEnergy() / eV);
            fAnalysisManager->AddNtupleRow(lowerSiPMHitNtupleID);
            lowerHitTimeAvg += hit->GetTime();
        }
        lowerHitTimeAvg /= ns * fSiPMPhotonHC.second->entries();
    }

    // Fill photoelectric response ntuple.
    constexpr size_t samplePoints = 512;
    constexpr G4double bufferTime = 1.0;
    constexpr G4double cutCoefficient = 5.0;
    constexpr G4double timeWindow = 0.25;   // SiPM single photoelectric response time window.
    G4double startTime, endTime;
    if (emptyUpperHC) {
        startTime = std::max(0.0, lowerFirstHitTime - bufferTime);
        endTime = cutCoefficient * lowerHitTimeAvg;
    } else if (emptyLowerHC) {
        startTime = std::max(0.0, upperFirstHitTime - bufferTime);
        endTime = cutCoefficient * upperHitTimeAvg;
    } else {
        startTime = std::max(0.0, std::min(upperFirstHitTime, lowerFirstHitTime) - bufferTime);
        endTime = 5.0 * std::max(upperHitTimeAvg, lowerHitTimeAvg);
    }
    G4double timeStep = (endTime - startTime) / (samplePoints - 1);

    size_t upperHCStartIndex = 0;
    size_t upperHCEndIndex = 0;
    size_t lowerHCStartIndex = 0;
    size_t lowerHCEndIndex = 0;
    G4double currentTime = startTime;
    for (size_t i = 0; i < samplePoints; ++i) {
        fAnalysisManager->FillNtupleDColumn(SiPMPhotoelectricResponseNtupleID, 0, currentTime);

        G4double windowBeginTime = std::max(startTime, currentTime - timeWindow);

        if (!emptyUpperHC) {
            while (static_cast<sbSiPMHit*>(fSiPMPhotonHC.first->
                GetHit(upperHCStartIndex))->GetTime() / ns < windowBeginTime) {
                if (upperHCStartIndex == fSiPMPhotonHC.first->entries()) {
                    break;
                }
                ++upperHCStartIndex;
            }
            while (static_cast<sbSiPMHit*>(fSiPMPhotonHC.first->
                GetHit(upperHCEndIndex))->GetTime() / ns < currentTime) {
                if (upperHCEndIndex == fSiPMPhotonHC.first->entries()) {
                    break;
                }
                ++upperHCEndIndex;
            }
            G4double upperPhotoelectricResponse = 0.0;
            for (size_t j = upperHCStartIndex; j < upperHCEndIndex; ++j) {
                upperPhotoelectricResponse += SiPMSinglePhotoelectricResponse(
                    currentTime - static_cast<sbSiPMHit*>(fSiPMPhotonHC.first->GetHit(j))->GetTime() / ns
                );
            }
            fAnalysisManager->FillNtupleDColumn(SiPMPhotoelectricResponseNtupleID, 1, upperPhotoelectricResponse);
        }

        if (!emptyLowerHC) {
            while (static_cast<sbSiPMHit*>(fSiPMPhotonHC.second->
                GetHit(lowerHCStartIndex))->GetTime() / ns < windowBeginTime) {
                if (lowerHCStartIndex == fSiPMPhotonHC.second->entries()) {
                    break;
                }
                ++lowerHCStartIndex;
            }
            while (static_cast<sbSiPMHit*>(fSiPMPhotonHC.second->
                GetHit(lowerHCEndIndex))->GetTime() / ns < currentTime) {
                if (lowerHCEndIndex == fSiPMPhotonHC.second->entries()) {
                    break;
                }
                ++lowerHCEndIndex;
            }
            G4double lowerPhotoelectricResponse = 0.0;
            for (size_t j = lowerHCStartIndex; j < lowerHCEndIndex; ++j) {
                lowerPhotoelectricResponse += SiPMSinglePhotoelectricResponse(
                    currentTime - static_cast<sbSiPMHit*>(fSiPMPhotonHC.second->GetHit(j))->GetTime() / ns
                );
            }
            fAnalysisManager->FillNtupleDColumn(SiPMPhotoelectricResponseNtupleID, 2, lowerPhotoelectricResponse);
        }

        fAnalysisManager->AddNtupleRow(SiPMPhotoelectricResponseNtupleID);
        currentTime += timeStep;
    }

    G4cout << "done." << G4endl;
}

