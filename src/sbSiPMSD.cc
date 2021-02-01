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
    auto hit = new sbSiPMHit(preStepPoint->GetPhysicalVolume()->GetName());
    hit->SetTime(preStepPoint->GetGlobalTime());
    hit->SetEnergy(preStepPoint->GetTotalEnergy());
    hit->SetPostion(preStepPoint->GetPosition());
    fOpticalPhotonHitsCollection->insert(hit);
    std::cout << hit->GetEnergy() / eV << "eV "
        << "photon hit SiPM " << hit->GetSiPMID() << "!" << G4endl;
    return true;
}

void sbSiPMSD::EndOfEvent(G4HCofThisEvent*) {}

