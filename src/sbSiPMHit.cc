#include "sbSiPMHit.hh"
#include "sbDetectorConstruction.hh"

G4ThreadLocal G4Allocator<sbSiPMHit>* sbSiPMHitAllocator = nullptr;

sbSiPMHit::sbSiPMHit() :
    G4VHit(),
    fTime(0.0),
    fEnergy(0.0) {}

sbSiPMHit::sbSiPMHit(const sbSiPMHit& rhs) :
    G4VHit(),
    fTime(rhs.fTime),
    fEnergy(rhs.fEnergy){}

sbSiPMHit::~sbSiPMHit() {}

const sbSiPMHit& sbSiPMHit::operator=(const sbSiPMHit& rhs) {
    if (&rhs != this) {
        this->fTime = rhs.fTime;
        this->fEnergy = rhs.fEnergy;
    }
    return *this;
}

