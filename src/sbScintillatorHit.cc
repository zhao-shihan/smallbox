#include "sbScintillatorHit.hh"

G4ThreadLocal G4Allocator<sbScintillatorHit>* sbScintillatorHitAllocator = nullptr;

sbScintillatorHit::sbScintillatorHit() :
    G4VHit(),
    fScintillatorID(-1),
    fTime(0.0),
    fPosition(0.0),
    fKineticEnergy(0.0),
    fEnergyDeposition(0.0) {}

sbScintillatorHit::sbScintillatorHit(const G4String& scintillatorName) :
    G4VHit(),
    fTime(0.0),
    fPosition(0.0),
    fKineticEnergy(0.0),
    fEnergyDeposition(0.0) {
    if (scintillatorName == gScintillatorsName.first) {
        fScintillatorID = 1;
    } else if (scintillatorName == gScintillatorsName.second) {
        fScintillatorID = 2;
    } else {
        G4ExceptionDescription exceptout;
        exceptout << "Scintillator named \"" + scintillatorName + "\" not found." << G4endl;
        exceptout << "This fScintillatorID will be set to -1." << G4endl;
        G4Exception(
            "sbScintillatorHit::sbScintillatorHit(const G4String& scintillatorName)",
            "ScintillatorNameNotFound",
            JustWarning,
            exceptout
        );
        fScintillatorID = -1;
    }
}

sbScintillatorHit::sbScintillatorHit(const sbScintillatorHit& rhs) :
    G4VHit(),
    fScintillatorID(rhs.fScintillatorID),
    fTime(rhs.fTime),
    fPosition(rhs.fPosition),
    fKineticEnergy(rhs.fKineticEnergy),
    fEnergyDeposition(rhs.fEnergyDeposition) {}

sbScintillatorHit::~sbScintillatorHit() {}

const sbScintillatorHit& sbScintillatorHit::operator=(const sbScintillatorHit& rhs) {
    if (&rhs != this) {
        this->fScintillatorID = rhs.fScintillatorID;
        this->fTime = rhs.fTime;
        this->fPosition = rhs.fPosition;
        this->fKineticEnergy = rhs.fKineticEnergy;
        this->fEnergyDeposition = rhs.fEnergyDeposition;
    }
    return *this;
}

