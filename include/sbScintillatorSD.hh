#ifndef SB_SCINTILLATOR_SD_H
#define SB_SCINTILLATOR_SD_H 1

#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "g4analysis.hh"

#include "sbScintillatorHit.hh"

class sbScintillatorSD : public G4VSensitiveDetector {
private:
    sbScintillatorHitsCollection* fMuonHitsCollection;

    G4ToolsAnalysisManager* fAnalysisManager;

public:
    sbScintillatorSD(const G4String& scintillatorSDName);
    virtual ~sbScintillatorSD();

    virtual void Initialize(G4HCofThisEvent* eventHitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory*);
    virtual void EndOfEvent(G4HCofThisEvent*);

private:
    void FillHistrogram() const;
};

#endif

