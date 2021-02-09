#include "sbSiPMHit.hh"
#include "sbDetectorConstruction.hh"

G4ThreadLocal G4Allocator<sbSiPMHit>* sbSiPMHitAllocator = nullptr;

sbSiPMHit::sbSiPMHit() :
    G4VHit(),
    fSiPMID(-1),
    fTime(0.0),
    fEnergy(0.0),
    fPosition(0.0) {}

sbSiPMHit::sbSiPMHit(G4VPhysicalVolume* physicalSiPM) :
    G4VHit(),
    fTime(0.0),
    fEnergy(0.0),
    fPosition(0.0) {
    auto sbDC = sbDetectorConstruction::GetsbDCInstance();
    if (physicalSiPM == sbDC->GetPhysicalSiPMs().first) {
        fSiPMID = fUpperSiPM;
    } else if (physicalSiPM == sbDC->GetPhysicalSiPMs().second) {
        fSiPMID = fLowerSiPM;
    } else {
        G4ExceptionDescription exceptout;
        exceptout << "The SiPM physical volume not found." << G4endl;
        exceptout << "This fSiPMID will be set to -1." << G4endl;
        exceptout << "Maybe you have changed the detector construction?" << G4endl;
        G4Exception(
            "sbSiPMHit::sbSiPMHit(G4VPhysicalVolume* physicalSiPM)",
            "SiPMNotFound",
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
    fEnergy(rhs.fEnergy),
    fPosition(rhs.fPosition) {}

sbSiPMHit::~sbSiPMHit() {}

const sbSiPMHit& sbSiPMHit::operator=(const sbSiPMHit& rhs) {
    if (&rhs != this) {
        this->fSiPMID = rhs.fSiPMID;
        this->fTime = rhs.fTime;
        this->fEnergy = rhs.fEnergy;
        this->fPosition = rhs.fPosition;
    }
    return *this;
}

