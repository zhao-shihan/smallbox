#include <sstream>
#include <mutex>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "sbSiPMSD.hh"
#include "sbRunAction.hh"
#include "sbDetectorConstruction.hh"

G4int sbSiPMSD::fHitEventCount = -1;
std::mutex sbSiPMSD::fMutex;

sbSiPMSD::sbSiPMSD(const G4String& SiPMSDName) :
    G4VSensitiveDetector(SiPMSDName),
    fSiPMPhotonHC(nullptr, nullptr),
    fAnalysisManager(nullptr) {
    if (gRunningInBatch) {
        fAnalysisManager = G4AnalysisManager::Instance();
    }
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

void sbSiPMSD::EndOfEvent(G4HCofThisEvent*) {
    if (!fSiPMPhotonHC.first || !fSiPMPhotonHC.second) {
        G4ExceptionDescription eout;
        eout << "SiPM optical photon hits collection is null." << G4endl;
        eout << "Maybe it was unexpectedly deleted?" << G4endl;
        G4Exception(
            "sbSiPMSD::EndOfEvent(G4HCofThisEvent*)",
            "HCNotFound",
            JustWarning,
            eout
        );
        return;
    }
    if (gRunningInBatch) {
        FillNtuple();
    }
}

void sbSiPMSD::FillNtuple() const {
    G4bool emptyUpperHC = fSiPMPhotonHC.first->entries() == 0;
    G4bool emptyLowerHC = fSiPMPhotonHC.second->entries() == 0;
    if (emptyUpperHC && emptyLowerHC) {
        return;
    }

    // Prevent multiple threads from getting the same ntupleID.
    fMutex.lock();
    ++fHitEventCount;
    G4int localHitEventCount = fHitEventCount;
    fMutex.unlock();

    G4cout << "sbSiPMSD::FillNtuple is processing "
        << fHitEventCount << "-th SiPM activated event ... ";

    constexpr G4int numOfNtuples = 3;
    G4int localCurrentFirstNtupleID = numOfNtuples * fHitEventCount;
    G4int upperSiPMHitNtupleID = localCurrentFirstNtupleID;
    G4int lowerSiPMHitNtupleID = upperSiPMHitNtupleID + 1;
    G4int SiPMPhotoelectricResponseNtupleID = lowerSiPMHitNtupleID + 1;

    std::vector<sbSiPMHit*> upperSiPMPhotonHitVec(fSiPMPhotonHC.first->entries());
    for (size_t i = 0; i < upperSiPMPhotonHitVec.size(); ++i) {
        upperSiPMPhotonHitVec[i] = static_cast<sbSiPMHit*>(fSiPMPhotonHC.first->GetHit(i));
    }
    std::sort(upperSiPMPhotonHitVec.begin(), upperSiPMPhotonHitVec.end(), compareHit);

    std::vector<sbSiPMHit*> lowerSiPMPhotonHitVec(fSiPMPhotonHC.second->entries());
    for (size_t i = 0; i < lowerSiPMPhotonHitVec.size(); ++i) {
        lowerSiPMPhotonHitVec[i] = static_cast<sbSiPMHit*>(fSiPMPhotonHC.second->GetHit(i));
    }
    std::sort(lowerSiPMPhotonHitVec.begin(), lowerSiPMPhotonHitVec.end(), compareHit);

    G4double upperFirstHitTime = 0.0;
    G4double upperHitTimeAvg = 0.0;
    if (!emptyUpperHC) {
        upperFirstHitTime = upperSiPMPhotonHitVec[0]->GetTime() / ns;
        // Fill upper hit ntuple, need units.
        // time in ns, energy in eV.
        for (const auto& hit : upperSiPMPhotonHitVec) {
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
        lowerFirstHitTime = lowerSiPMPhotonHitVec[0]->GetTime() / ns;
        // Fill lower hit ntuple.
        for (const auto& hit : lowerSiPMPhotonHitVec) {
            fAnalysisManager->FillNtupleDColumn(lowerSiPMHitNtupleID, 0, hit->GetTime() / ns);
            fAnalysisManager->FillNtupleDColumn(lowerSiPMHitNtupleID, 1, hit->GetEnergy() / eV);
            fAnalysisManager->AddNtupleRow(lowerSiPMHitNtupleID);
            lowerHitTimeAvg += hit->GetTime();
        }
        lowerHitTimeAvg /= ns * fSiPMPhotonHC.second->entries();
    }

    // Fill photoelectric response ntuple.
    constexpr size_t samplePoints = 1024;
    constexpr G4double bufferTime = 1.0;
    constexpr G4double cutCoefficient = 6.0;
    constexpr G4double timeWindow = 5.0;   // SiPM single photoelectric response time window.
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

    auto upperStartHit = upperSiPMPhotonHitVec.begin();
    auto upperEndHit = upperSiPMPhotonHitVec.begin();;
    auto lowerStartHit = lowerSiPMPhotonHitVec.begin();
    auto lowerEndHit = lowerSiPMPhotonHitVec.begin();;

    G4double currentTime = startTime;

    std::stringstream ss;
    std::string prCSVName;
    ss << gSiPMResultCSVDestDir << "/pr" << localHitEventCount << ".csv";
    ss >> prCSVName;
    std::ofstream prcsvout(prCSVName);

    if (!prcsvout.is_open()) {
        G4ExceptionDescription eout;
        eout << "Cannot open " + prCSVName << G4endl;
        G4Exception(
            "sbSiPMSD::FillNtuple()",
            "CannotOpenCSVFile",
            FatalException,
            eout
        );
        // Aborted.
    }

    // write csv column title
    prcsvout << "time(ns),UpperSiPMPhotoelectricResponse,LowerSiPMPhotoelectricResponse" << G4endl;

    for (size_t i = 0; i < samplePoints; ++i) {
        // write time stamp.
        fAnalysisManager->FillNtupleDColumn(SiPMPhotoelectricResponseNtupleID, 0, currentTime);
        prcsvout << currentTime;

        G4double windowBeginTime = std::max(startTime, currentTime - timeWindow);

        if (!emptyUpperHC) {
            while (upperStartHit != upperSiPMPhotonHitVec.end()) {
                if ((*upperStartHit)->GetTime() / ns > windowBeginTime) { break; }
                ++upperStartHit;
            }
            while (upperEndHit != upperSiPMPhotonHitVec.end()) {
                if ((*upperEndHit)->GetTime() / ns > currentTime) { break; }
                ++upperEndHit;
            }
            G4double upperPhotoelectricResponse = 0.0;
            for (auto hit = upperStartHit; hit != upperEndHit; ++hit) {
                upperPhotoelectricResponse += SiPMSinglePhotoelectricResponse(currentTime - (*hit)->GetTime() / ns);
            }
            // write upper SiPM photoelectric response.
            fAnalysisManager->FillNtupleDColumn(SiPMPhotoelectricResponseNtupleID, 1, upperPhotoelectricResponse);
            prcsvout << ',' << upperPhotoelectricResponse;
        }

        if (!emptyLowerHC) {
            while (lowerStartHit != lowerSiPMPhotonHitVec.end()) {
                if ((*lowerStartHit)->GetTime() / ns > windowBeginTime) { break; }
                ++lowerStartHit;
            }
            while (lowerEndHit != lowerSiPMPhotonHitVec.end()) {
                if ((*lowerEndHit)->GetTime() / ns > currentTime) { break; }
                ++lowerEndHit;
            }
            G4double lowerPhotoelectricResponse = 0.0;
            for (auto hit = lowerStartHit; hit != lowerEndHit; ++hit) {
                lowerPhotoelectricResponse += SiPMSinglePhotoelectricResponse(currentTime - (*hit)->GetTime() / ns);
            }
            // write lower SiPM photoelectric response.
            fAnalysisManager->FillNtupleDColumn(SiPMPhotoelectricResponseNtupleID, 2, lowerPhotoelectricResponse);
            prcsvout << ',' << lowerPhotoelectricResponse;
        }
        // Add a new row.
        fAnalysisManager->AddNtupleRow(SiPMPhotoelectricResponseNtupleID);
        prcsvout << G4endl;

        currentTime += timeStep;
    }

    G4cout << "done." << G4endl;
}

