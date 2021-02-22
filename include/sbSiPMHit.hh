#ifndef SB_SIPM_HIT_H
#define SB_SIPM_HIT_H 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"

#include "sbGlobal.hh"

class sbSiPMHit : public G4VHit {
private:
    G4double      fTime;
    G4double      fEnergy;

public:
    static enum sbSiPMSet {
        fUpperSiPM,
        fLowerSiPM
    } fSiPMSet;

public:
    sbSiPMHit();
    sbSiPMHit(const sbSiPMHit& rhs);
    ~sbSiPMHit();
    const sbSiPMHit& operator=(const sbSiPMHit& rhs);
    G4bool operator==(const sbSiPMHit& rhs) const { return this == &rhs; }
    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    const G4double& GetTime() const { return fTime; }
    const G4double& GetEnergy() const { return fEnergy; }

    void SetTime(const G4double& time) { fTime = time; }
    void SetEnergy(const G4double& energy) { fEnergy = energy; }
};

typedef G4THitsCollection<sbSiPMHit> sbSiPMHitsCollection;

extern G4ThreadLocal G4Allocator<sbSiPMHit>* sbSiPMHitAllocator;

inline void* sbSiPMHit::operator new(size_t) {
    if (!sbSiPMHitAllocator) {
        sbSiPMHitAllocator = new G4Allocator<sbSiPMHit>();
    }
    return (void*)sbSiPMHitAllocator->MallocSingle();
}

inline void sbSiPMHit::operator delete(void* aHit) {
    sbSiPMHitAllocator->FreeSingle(static_cast<sbSiPMHit*>(aHit));
}

#endif

