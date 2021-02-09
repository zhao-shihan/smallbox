#include "g4analysis.hh"

#include "sbSiPMSD.hh"

sbSiPMSD::sbSiPMSD(const G4String& SiPMSDName) :
    G4VSensitiveDetector(SiPMSDName),
    fOpticalPhotonHitsCollection(nullptr) {
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
    hit->SetEnergy(preStepPoint->GetTotalEnergy() / eV);
    hit->SetPostion(preStepPoint->GetPosition());
    fOpticalPhotonHitsCollection->insert(hit);
    G4cout << step->GetTrack()->GetTrackID() << G4endl;
    step->GetTrack()->SetTrackStatus(fStopAndKill);
    return true;
}

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

    auto analysisManager = G4AnalysisManager::Instance();
    for (size_t i = 0; i < fOpticalPhotonHitsCollection->entries(); ++i) {
        auto hit = static_cast<sbSiPMHit*>(fOpticalPhotonHitsCollection->GetHit(i));

        //Fill histogram and ntuple
        analysisManager->FillNtupleDColumn(hit->GetSiPMID(), 0, hit->GetTime());
        analysisManager->FillNtupleDColumn(hit->GetSiPMID(), 1, hit->GetEnergy());
        analysisManager->AddNtupleRow(hit->GetSiPMID());
    }
}

