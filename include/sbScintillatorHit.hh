#ifndef SB_SCINTILLATOR_HIT_H
#define SB_SCINTILLATOR_HIT_H 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include "sbGlobal.hh"

class sbScintillatorHit : public G4VHit {
private:
    G4int fScintillatorID;
    G4double fTime;
    G4ThreeVector fPosition;
    G4double fEnergyDeposition;

public:
    sbScintillatorHit();
    sbScintillatorHit(const G4String& scintillatorName);
    sbScintillatorHit(const sbScintillatorHit& rhs);
    ~sbScintillatorHit();
    const sbScintillatorHit& operator=(const sbScintillatorHit& rhs);
    G4bool operator==(const sbScintillatorHit& rhs) const { return this == &rhs; }
    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    G4int GetScintillatorID() const { return fScintillatorID; }
    inline G4String GetScintillatorName() const;
    void SetScintillatorID(const G4int& scintillatorID) { fScintillatorID = scintillatorID; }
    G4double GetTime() const { return fTime; }
    void SetTime(const G4double& time) { fTime = time; }
    G4ThreeVector GetPosition() const { return fPosition; }
    void SetPostion(const G4ThreeVector& position) { fPosition = position; }
    G4double GetEnergyDeposition() const { return fEnergyDeposition; }
    void SetEnergyDeposition(const G4double& energyDeposition) { fEnergyDeposition = energyDeposition; }
};

typedef G4THitsCollection<sbScintillatorHit> sbScintillatorHitsCollection;

extern G4ThreadLocal G4Allocator<sbScintillatorHit>* sbScintillatorHitAllocator;

inline void* sbScintillatorHit::operator new(size_t) {
    if (!sbScintillatorHitAllocator) {
        sbScintillatorHitAllocator = new G4Allocator<sbScintillatorHit>();
    }
    return (void*)sbScintillatorHitAllocator->MallocSingle();
}

inline void sbScintillatorHit::operator delete(void* aHit) {
    sbScintillatorHitAllocator->FreeSingle((sbScintillatorHit*)aHit);
}

inline G4String sbScintillatorHit::GetScintillatorName() const {
    if (this->fScintillatorID == 1) {
        return gScintillatorsName.first;
    } else if (this->fScintillatorID == 2) {
        return gScintillatorsName.second;
    } else {
        G4ExceptionDescription exceptout;
        exceptout << "This fScintillatorID is -1," << G4endl;
        exceptout << "which means scintillator of this hit is not defined." << G4endl;
        exceptout << "Will return \"unknown\"." << G4endl;
        G4Exception(
            "sbScintillatorHit::GetScintillatorName()",
            "ScintillatorNameNotFound",
            JustWarning,
            exceptout
        );
        return "unknown";
    }
}

#endif

