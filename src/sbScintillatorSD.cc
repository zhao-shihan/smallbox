#include "sbScintillatorSD.hh"

sbScintillatorSD::sbScintillatorSD(const G4String& sensitiveDetectorName) :
    G4VSensitiveDetector(sensitiveDetectorName),
    fMuonHitsCollection(nullptr) {
    collectionName.push_back("muon_hits_collection");
}

sbScintillatorSD::~sbScintillatorSD() {}

void sbScintillatorSD::Initialize(G4HCofThisEvent* hitCollectionOfThisEvent) {
    fMuonHitsCollection = new sbScintillatorHitsCollection(SensitiveDetectorName, collectionName[0]);
    hitCollectionOfThisEvent->AddHitsCollection(GetCollectionID(0), fMuonHitsCollection);
}

G4bool sbScintillatorSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    auto presentParticle = step->GetTrack()->GetDefinition();
    if (presentParticle->GetParticleName() == "mu+" ||
        presentParticle->GetParticleName() == "mu-") {
        // Present step point.
        auto preStepPoint = step->GetPreStepPoint();
        // A new hit.
        auto hit = new sbScintillatorHit(preStepPoint->GetPhysicalVolume()->GetName());
        hit->SetPostion(preStepPoint->GetPosition());
        hit->SetTime(preStepPoint->GetGlobalTime());
        fMuonHitsCollection->insert(hit);
        std::cout << "fuck" << std::endl;
        return true;
    } else { return false; }
}

