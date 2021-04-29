#include "SBSiPMHit.hh"

G4ThreadLocal G4Allocator<SBSiPMHit>* SBSiPMHitAllocator = nullptr;

SBSiPMHit::SBSiPMHit() :
    G4VHit(),
    fTime(0.0),
    fEnergy(0.0) {}

SBSiPMHit::SBSiPMHit(G4double time, G4double energy):
    G4VHit(),
    fTime(time),
    fEnergy(energy) {}

SBSiPMHit::SBSiPMHit(const SBSiPMHit& rhs) :
    G4VHit(),
    fTime(rhs.fTime),
    fEnergy(rhs.fEnergy){}

SBSiPMHit::~SBSiPMHit() {}

const SBSiPMHit& SBSiPMHit::operator=(const SBSiPMHit& rhs) {
    if (&rhs != this) {
        this->fTime = rhs.fTime;
        this->fEnergy = rhs.fEnergy;
    }
    return *this;
}

