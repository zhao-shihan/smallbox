#ifndef SB_TRACKER_HIT_H
#define SB_TRACKER_HIT_H

#include <iomanip>

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEnergyDeposite, fPosition

class sbTrackerHit : public G4VHit {
public:
    sbTrackerHit();
    sbTrackerHit(const sbTrackerHit&);
    virtual ~sbTrackerHit();

    // operators
    const sbTrackerHit& operator=(const sbTrackerHit&);
    G4bool operator==(const sbTrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID(const G4int& track) { fTrackID = track; };
    void SetChamberNb(const G4int& chamb) { fChamberNb = chamb; };
    void SetEdep(const G4double& deposite) { fEnergyDeposite = deposite; };
    void SetPos(const G4ThreeVector& xyz) { fPosition = xyz; };

    // Get methods
    G4int         GetTrackID() const { return fTrackID; };
    G4int         GetChamberNb() const { return fChamberNb; };
    G4double      GetEnergyDeposite() const { return fEnergyDeposite; };
    G4ThreeVector GetPosition() const { return fPosition; };

private:
    G4int         fTrackID;
    G4int         fChamberNb;
    G4double      fEnergyDeposite;
    G4ThreeVector fPosition;
};

typedef G4THitsCollection<sbTrackerHit> sbTrackerHitsCollection;

extern G4ThreadLocal G4Allocator<sbTrackerHit>* sbTrackerHitAllocator;

inline void* sbTrackerHit::operator new(size_t) {
    if (!sbTrackerHitAllocator) {
        sbTrackerHitAllocator = new G4Allocator<sbTrackerHit>;
    }
    return (void*)sbTrackerHitAllocator->MallocSingle();
}

inline void sbTrackerHit::operator delete(void* hit) {
    sbTrackerHitAllocator->FreeSingle((sbTrackerHit*)hit);
}

#endif
