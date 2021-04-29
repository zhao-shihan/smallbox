#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4OpticalPhoton.hh"
#include "G4MTRunManager.hh"

#include "SBScintillatorSD.hh"
#include "SBScintillatorHit.hh"

SBScintillatorSD::SBScintillatorSD(const G4String& scintillatorSDName) :
    G4VSensitiveDetector(scintillatorSDName),
    fMuonHitsCollection(nullptr),
    fAnalysisManager(new SBAnalysisManager()),
    fDetectorConstruction(static_cast<const SBDetectorConstruction*>(
        G4MTRunManager::GetMasterRunManager()->GetUserDetectorConstruction())) {
    collectionName.push_back("muon_hits_collection");
}

SBScintillatorSD::~SBScintillatorSD() {
    delete fAnalysisManager;
}

void SBScintillatorSD::Initialize(G4HCofThisEvent* hitCollectionOfThisEvent) {
    fMuonHitsCollection = new SBScintillatorHitsCollection(SensitiveDetectorName, collectionName[0]);
    hitCollectionOfThisEvent->AddHitsCollection(GetCollectionID(0), fMuonHitsCollection);
}

G4bool SBScintillatorSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    if (step->GetTrack()->GetParticleDefinition() != G4MuonPlus::Definition() &&
        step->GetTrack()->GetParticleDefinition() != G4MuonMinus::Definition()) {
        return false;
    }
    if (!step->IsFirstStepInVolume()) { return false; }
    const auto& position = step->GetPreStepPoint()->GetPosition();
    const auto& momentum = step->GetPreStepPoint()->GetMomentum();
    if (momentum.z() != 0) {
        auto hit = new SBScintillatorHit(
            position - momentum * (fDetectorConstruction->GetWorldZMax() / momentum.z()),
            momentum,
            step->GetPreStepPoint()->GetKineticEnergy()
        );
        fMuonHitsCollection->insert(hit);
    } else {
        auto hit = new SBScintillatorHit(
            position,
            momentum,
            step->GetPreStepPoint()->GetKineticEnergy()
        );
        fMuonHitsCollection->insert(hit);
    }
    return true;
}

void SBScintillatorSD::EndOfEvent(G4HCofThisEvent*) {
#if SB_SAVE_ANYTHING
    if (fMuonHitsCollection->entries() == 0) {
        fAnalysisManager->FillMuonHitInfo(0.0, 0.0, 0.0, 0.0, 0.0);
    } else {
        auto hit = static_cast<SBScintillatorHit*>(fMuonHitsCollection->GetHit(0));
        const auto& position = hit->GetPosition();
        const auto& momentum = hit->GetMomentum();
        fAnalysisManager->FillMuonHitInfo(position.x(), position.y(),
            M_PI + momentum.phi(), M_PI - momentum.theta(), hit->GetKineticEnergy());
    }
#endif
}

