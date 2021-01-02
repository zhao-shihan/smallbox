#ifndef SB_TRACKERSD_H
#define SB_TRACKERSD_H

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "sbTrackerHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

/// sbTracker sensitive detector class
///
/// The hits are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step. A hit is created with each step with non zero 
/// energy deposit.

class sbTrackerSD : public G4VSensitiveDetector {
public:
    sbTrackerSD(const G4String& name, const G4String& hitsCollectionName);
    virtual ~sbTrackerSD();

    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

private:
    sbTrackerHitsCollection* fHitsCollection;
};

#endif
