#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4OpticalPhoton.hh"

#include "sbScintillatorSD.hh"
#include "sbConfigs.hh"

sbScintillatorSD::sbScintillatorSD(const G4String& scintillatorSDName) :
    G4VSensitiveDetector(scintillatorSDName),
    fMuonHitsCollection(nullptr) {
    collectionName.push_back("muon_hits_collection");
}

sbScintillatorSD::~sbScintillatorSD() {}

void sbScintillatorSD::Initialize(G4HCofThisEvent* hitCollectionOfThisEvent) {
    fMuonHitsCollection = new sbScintillatorHitsCollection(SensitiveDetectorName, collectionName[0]);
    hitCollectionOfThisEvent->AddHitsCollection(GetCollectionID(0), fMuonHitsCollection);
}

G4bool sbScintillatorSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    auto presentParticle = step->GetTrack()->GetParticleDefinition();
    if (presentParticle != G4MuonPlus::Definition() &&
        presentParticle != G4MuonMinus::Definition()) {
        return false;
    }
    if (!step->IsFirstStepInVolume()) { return false; }
    // Present step point.
    auto preStepPoint = step->GetPreStepPoint();
    // A new hit.
    auto hit = new sbScintillatorHit(preStepPoint->GetPhysicalVolume());
    hit->SetPostion(preStepPoint->GetPosition());
    hit->SetTime(preStepPoint->GetGlobalTime());
    hit->SetKineticEnergy(preStepPoint->GetKineticEnergy());
    fMuonHitsCollection->insert(hit);
    if (presentParticle == G4MuonPlus::Definition()) {
        G4cout << "mu+ hit scintillator " << hit->GetScintillatorID()
            << " at " << hit->GetTime() << "ns with "
            << hit->GetKineticEnergy() / GeV << "GeV" << G4endl;
    } else {
        G4cout << "mu- hit scintillator " << hit->GetScintillatorID()
            << " at " << hit->GetTime() << "ns with "
            << hit->GetKineticEnergy() / GeV << "GeV" << G4endl;
    }
    return true;
}

void sbScintillatorSD::EndOfEvent(G4HCofThisEvent*) {}


