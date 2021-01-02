#include "sbTrackerHit.hh"

G4ThreadLocal G4Allocator<sbTrackerHit>* sbTrackerHitAllocator = 0;

sbTrackerHit::sbTrackerHit()
    : G4VHit(),
    fTrackID(-1),
    fChamberNb(-1),
    fEnergyDeposite(0.),
    fPosition(G4ThreeVector()) {}

sbTrackerHit::~sbTrackerHit() {}

sbTrackerHit::sbTrackerHit(const sbTrackerHit& right)
    : G4VHit() {
    fTrackID = right.fTrackID;
    fChamberNb = right.fChamberNb;
    fEnergyDeposite = right.fEnergyDeposite;
    fPosition = right.fPosition;
}

const sbTrackerHit& sbTrackerHit::operator=(const sbTrackerHit& right) {
    fTrackID = right.fTrackID;
    fChamberNb = right.fChamberNb;
    fEnergyDeposite = right.fEnergyDeposite;
    fPosition = right.fPosition;

    return *this;
}

G4bool sbTrackerHit::operator==(const sbTrackerHit& right) const {
    return this == &right;
}

void sbTrackerHit::Draw() {
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (pVVisManager) {
        G4Circle circle(fPosition);
        circle.SetScreenSize(4.);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1., 0., 0.);
        G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        pVVisManager->Draw(circle);
    }
}

void sbTrackerHit::Print() {
    G4cout
        << "  trackID: " << fTrackID << " chamberNb: " << fChamberNb
        << "Edep: "
        << std::setw(7) << G4BestUnit(fEnergyDeposite, "Energy")
        << " Position: "
        << std::setw(7) << G4BestUnit(fPosition, "Length")
        << G4endl;
}
