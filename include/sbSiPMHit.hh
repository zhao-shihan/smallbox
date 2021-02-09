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
    G4int         fSiPMID;
    G4double      fTime;
    G4double      fEnergy;
    G4ThreeVector fPosition;

private:
    static enum { fUpperSiPM, fLowerSiPM } fSiPMIDSet;

public:
    sbSiPMHit();
    sbSiPMHit(G4VPhysicalVolume* physicalSiPM);
    sbSiPMHit(const sbSiPMHit& rhs);
    ~sbSiPMHit();
    const sbSiPMHit& operator=(const sbSiPMHit& rhs);
    G4bool operator==(const sbSiPMHit& rhs) const { return this == &rhs; }
    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    G4int           GetSiPMID() const { return fSiPMID; }
    inline G4String GetSiPMName() const;
    G4double        GetTime() const { return fTime; }
    G4double        GetEnergy() const { return fEnergy; }
    G4ThreeVector   GetPosition() const { return fPosition; }

    void            SetSiPMID(const G4int& SiPMID) { fSiPMID = SiPMID; }
    void            SetTime(const G4double& time) { fTime = time; }
    void            SetEnergy(const G4double& energy) { fEnergy = energy; }
    void            SetPostion(const G4ThreeVector& position) { fPosition = position; }
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

inline G4String sbSiPMHit::GetSiPMName() const {
    if (this->fSiPMID == fUpperSiPM) {
        return gSiPMsName.first;
    } else if (this->fSiPMID == fLowerSiPM) {
        return gSiPMsName.second;
    } else {
        G4ExceptionDescription exceptout;
        exceptout << "This fSiPMID is -1," << G4endl;
        exceptout << "which means SiPM of this hit is not defined." << G4endl;
        exceptout << "Will return \"unknown\"." << G4endl;
        G4Exception(
            "sbSiPMHit::GetSiPMName()",
            "SiPMNameNotFound",
            JustWarning,
            exceptout
        );
        return "unknown";
    }
}

#endif

