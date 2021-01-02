#include "sbTrackerSD.hh"

sbTrackerSD::sbTrackerSD(const G4String& name, const G4String& hitsCollectionName)
    : G4VSensitiveDetector(name),
    fHitsCollection(nullptr) {
    collectionName.insert(hitsCollectionName);
}

sbTrackerSD::~sbTrackerSD() {}

void sbTrackerSD::Initialize(G4HCofThisEvent* hce) {
    // Create hits collection
    //
    fHitsCollection = new sbTrackerHitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hce
    //
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool sbTrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    // energy deposit
    G4double edep = aStep->GetTotalEnergyDeposit();

    if (edep == 0.) { return false; }

    sbTrackerHit* newHit = new sbTrackerHit();

    newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
    newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    newHit->SetEdep(edep);
    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());

    fHitsCollection->insert(newHit);

    //newHit->Print();

    return true;
}

void sbTrackerSD::EndOfEvent(G4HCofThisEvent*) {
    if (verboseLevel > 1) {
        G4int nofHits = fHitsCollection->entries();
        G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits
            << " hits in the tracker chambers: " << G4endl;
        for (G4int i = 0; i < nofHits; i++) (*fHitsCollection)[i]->Print();
    }
}
