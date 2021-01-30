#ifndef SB_SIPM_SD_H
#define SB_SIPM_SD_H 1

#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4OpticalPhoton.hh"

#include "sbSiPMHit.hh"

class sbSiPMSD : public G4VSensitiveDetector {
private:
    sbSiPMHitsCollection* fOpticalPhotonHitsCollection;

public:
    sbSiPMSD(const G4String& SiPMSDName);
    virtual ~sbSiPMSD();

    virtual void Initialize(G4HCofThisEvent* eventHitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory*);
    virtual void EndOfEvent(G4HCofThisEvent*);
};

#endif

