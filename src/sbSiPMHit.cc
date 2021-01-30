#include "sbSiPMHit.hh"

G4ThreadLocal G4Allocator<sbSiPMHit>* sbSiPMHitAllocator = nullptr;

sbSiPMHit::sbSiPMHit() :
    G4VHit(),
    fSiPMID(-1),
    fTime(0.0),
    fPosition(0.0) {}

sbSiPMHit::sbSiPMHit(const G4String& SiPMName) :
    G4VHit(),
    fTime(0.0),
    fPosition(0.0) {
    if (SiPMName == gSiPMsName.first) {
        fSiPMID = 1;
    } else if (SiPMName == gSiPMsName.second) {
        fSiPMID = 2;
    } else {
        G4ExceptionDescription exceptout;
        exceptout << "SiPM named \"" + SiPMName + "\" not found." << G4endl;
        exceptout << "This fSiPMID will be set to -1." << G4endl;
        G4Exception(
            "sbSiPMHit::sbSiPMHit(const G4String& SiPMName)",
            "SiPMNameNotFound",
            JustWarning,
            exceptout
        );
        fSiPMID = -1;
    }
}

sbSiPMHit::sbSiPMHit(const sbSiPMHit& rhs) :
    G4VHit(),
    fSiPMID(rhs.fSiPMID),
    fTime(rhs.fTime),
    fPosition(rhs.fPosition) {}

sbSiPMHit::~sbSiPMHit() {}

const sbSiPMHit& sbSiPMHit::operator=(const sbSiPMHit& rhs) {
    if (&rhs != this) {
        this->fSiPMID = rhs.fSiPMID;
        this->fTime = rhs.fTime;
        this->fPosition = rhs.fPosition;
    }
    return *this;
}

