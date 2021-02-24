#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4OpticalPhoton.hh"

#include "sbScintillatorSD.hh"
#include "sbConfigs.hh"
#include "sbScintillatorHit.hh"

sbScintillatorSD::sbScintillatorSD(const G4String& scintillatorSDName) :
    G4VSensitiveDetector(scintillatorSDName),
    fMuonHitsCollection(nullptr),
    fAnalysisManager(nullptr) {
    if (gRunningInBatch) {
        fAnalysisManager = G4AnalysisManager::Instance();
    }
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
    hit->SetPosition(preStepPoint->GetPosition());
    hit->SetTime(preStepPoint->GetGlobalTime());
    hit->SetKineticEnergy(preStepPoint->GetKineticEnergy());
    hit->SetMomentumDirection(preStepPoint->GetMomentumDirection());
    hit->SetEnergyDeposition(step->GetTotalEnergyDeposit());
    hit->SetParticleDefinition(presentParticle);
    fMuonHitsCollection->insert(hit);
    return true;
}

void sbScintillatorSD::EndOfEvent(G4HCofThisEvent*) {
    if (!fMuonHitsCollection) {
        G4ExceptionDescription exceptout;
        exceptout << "fMuonHitsCollection is nullptr." << G4endl;
        exceptout << "Maybe it was unexpectedly deleted?" << G4endl;
        G4Exception(
            "sbScintillatorSD::EndOfEvent(G4HCofThisEvent*)",
            "HCNotFound",
            JustWarning,
            exceptout
        );
        return;
    }
    if (gRunningInBatch) {
        FillHistrogram();
    }
}

void sbScintillatorSD::FillHistrogram() const {
    for (size_t i = 0; i < fMuonHitsCollection->entries(); ++i) {
        auto hit = static_cast<sbScintillatorHit*>(fMuonHitsCollection->GetHit(i));
        // Fill histrogram, no need of units.
        G4int HistID = 0;
        if (hit->GetScintillatorID() == sbScintillatorHit::sbScintillatorSet::fLowerScintillator) {
            ++HistID;
        }
        fAnalysisManager->FillH1(HistID, hit->GetKineticEnergy());
        HistID += 2;
        if (hit->GetParticleDefinition() == G4MuonPlus::Definition()) {
            fAnalysisManager->FillH1(HistID, hit->GetKineticEnergy());
        } else {
            HistID += 2;
            fAnalysisManager->FillH1(HistID, hit->GetKineticEnergy());
        }
    }
}

