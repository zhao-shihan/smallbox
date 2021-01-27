#ifndef SB_SCINTILLATOR_H
#define SB_SCINTILLATOR_H 1

#include "G4VSensitiveDetector.hh"
#include "G4String.hh"

class sbScintillatorSD : public G4VSensitiveDetector {
public:
    sbScintillatorSD(const G4String& hitCollectionName);
};

#endif

