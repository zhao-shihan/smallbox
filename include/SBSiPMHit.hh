#ifndef SB_SIPM_HIT_H
#define SB_SIPM_HIT_H 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class SBSiPMHit : public G4VHit {
private:
    G4double fTime;
    G4double fEnergy;

public:
    SBSiPMHit();
    SBSiPMHit(G4double time, G4double energy);
    SBSiPMHit(const SBSiPMHit& rhs);
    ~SBSiPMHit();
    const SBSiPMHit& operator=(const SBSiPMHit& rhs);
    G4bool operator==(const SBSiPMHit& rhs) const { return this == &rhs; }
    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    const G4double& GetTime() const { return fTime; }
    const G4double& GetEnergy() const { return fEnergy; }

    void SetTime(const G4double& time) { fTime = time; }
    void SetEnergy(const G4double& energy) { fEnergy = energy; }
};

typedef G4THitsCollection<SBSiPMHit> SBSiPMHitsCollection;

extern G4ThreadLocal G4Allocator<SBSiPMHit>* SBSiPMHitAllocator;

inline void* SBSiPMHit::operator new(size_t) {
    if (!SBSiPMHitAllocator) {
        SBSiPMHitAllocator = new G4Allocator<SBSiPMHit>();
    }
    return static_cast<void*>(SBSiPMHitAllocator->MallocSingle());
}

inline void SBSiPMHit::operator delete(void* aHit) {
    SBSiPMHitAllocator->FreeSingle(static_cast<SBSiPMHit*>(aHit));
}

#endif

