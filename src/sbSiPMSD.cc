#include <sstream>
#include <mutex>

#include "sbSiPMSD.hh"
#include "sbRunAction.hh"

G4int sbSiPMSD::fCurrentNtupleID = -1;

sbSiPMSD::sbSiPMSD(const G4String& SiPMSDName) :
    G4VSensitiveDetector(SiPMSDName),
    fOpticalPhotonHitsCollection(nullptr),
    fAnalysisManager(G4AnalysisManager::Instance()) {
    collectionName.push_back("optical_photon_hits_collection");
}

sbSiPMSD::~sbSiPMSD() {}

void sbSiPMSD::Initialize(G4HCofThisEvent* hitCollectionOfThisEvent) {
    fOpticalPhotonHitsCollection = new sbSiPMHitsCollection(SensitiveDetectorName, collectionName[0]);
    hitCollectionOfThisEvent->AddHitsCollection(GetCollectionID(0), fOpticalPhotonHitsCollection);
}

G4bool sbSiPMSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    auto presentParticle = step->GetTrack()->GetParticleDefinition();
    if (presentParticle != G4OpticalPhoton::Definition()) {
        return false;
    }
    // Present step point.
    auto preStepPoint = step->GetPreStepPoint();
    // A new hit.
    auto hit = new sbSiPMHit(preStepPoint->GetPhysicalVolume());
    hit->SetTime(preStepPoint->GetGlobalTime());
    hit->SetEnergy(preStepPoint->GetTotalEnergy());
    hit->SetPostion(preStepPoint->GetPosition());
    fOpticalPhotonHitsCollection->insert(hit);
    return true;
}

std::mutex mtx;

void sbSiPMSD::EndOfEvent(G4HCofThisEvent*) {
    if (!fOpticalPhotonHitsCollection) {
        G4ExceptionDescription exceptout;
        exceptout << "fOpticalPhotonHitsCollection is nullptr." << G4endl;
        exceptout << "Maybe it was unexpectedly deleted?" << G4endl;
        G4Exception(
            "sbSiPMSD::EndOfEvent(G4HCofThisEvent*)",
            "HCNotFound",
            JustWarning,
            exceptout
        );
        return;
    }
    if (fOpticalPhotonHitsCollection->entries() == 0) {
        return;
    }

    // Prevent multiple threads from getting the same ntupleID.
    mtx.lock();
    fCurrentNtupleID += 2;
    G4int upperSiPMntupleID = fCurrentNtupleID - 1;
    G4int lowerSiPMntupleID = fCurrentNtupleID;
    mtx.unlock();

    G4cout << "sbSiPMSD::EndOfEvent is filling      ntuple "
        << upperSiPMntupleID << " & " << lowerSiPMntupleID << " ... ";

    for (size_t i = 0; i < fOpticalPhotonHitsCollection->entries(); ++i) {
        auto hit = static_cast<sbSiPMHit*>(fOpticalPhotonHitsCollection->GetHit(i));
        // Fill ntuple, need units.
        if (hit->GetSiPMID() == sbSiPMHit::sbSiPMSet::fUpperSiPM) {
            fAnalysisManager->FillNtupleDColumn(upperSiPMntupleID, 0, hit->GetTime() / ns);
            fAnalysisManager->FillNtupleDColumn(upperSiPMntupleID, 1, hit->GetEnergy() / eV);
            fAnalysisManager->AddNtupleRow(upperSiPMntupleID);
        } else {
            fAnalysisManager->FillNtupleDColumn(lowerSiPMntupleID, 0, hit->GetTime() / ns);
            fAnalysisManager->FillNtupleDColumn(lowerSiPMntupleID, 1, hit->GetEnergy() / eV);
            fAnalysisManager->AddNtupleRow(lowerSiPMntupleID);
        }
    }

    G4cout << "done." << G4endl;
}

