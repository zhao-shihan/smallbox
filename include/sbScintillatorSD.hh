#ifndef SB_SCINTILLATOR_SD_H
#define SB_SCINTILLATOR_SD_H 1

#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

#include "sbScintillatorHit.hh"

class sbScintillatorSD : public G4VSensitiveDetector {
private:
    sbScintillatorHitsCollection* fMuonHitsCollection;

public:
    sbScintillatorSD(const G4String& scintillatorSDName);
    virtual ~sbScintillatorSD();

    virtual void Initialize(G4HCofThisEvent* eventHitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory*);
    virtual void EndOfEvent(G4HCofThisEvent*);
};

#endif

