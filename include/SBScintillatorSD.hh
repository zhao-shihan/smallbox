#ifndef SB_SCINTILLATOR_SD_H
#define SB_SCINTILLATOR_SD_H 1

#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"

#include "SBScintillatorHit.hh"
#include "SBAnalysisManager.hh"
#include "SBDetectorConstruction.hh"

class SBDetectorConstruction;

class SBScintillatorSD : public G4VSensitiveDetector {
private:
    SBScintillatorHitsCollection* fMuonHitsCollection;
    SBAnalysisManager* fAnalysisManager;

    const SBDetectorConstruction* fDetectorConstruction;

public:
    SBScintillatorSD(const G4String& scintillatorSDName);
    virtual ~SBScintillatorSD();

    virtual void Initialize(G4HCofThisEvent* eventHitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory*);
    virtual void EndOfEvent(G4HCofThisEvent*);
};

#endif

