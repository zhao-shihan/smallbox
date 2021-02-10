#ifndef SB_SCINTILLATOR_HIT_H
#define SB_SCINTILLATOR_HIT_H 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"

#include "sbGlobal.hh"

class sbScintillatorHit : public G4VHit {
private:
    G4int         fScintillatorID;
    G4double      fTime;
    G4ThreeVector fPosition;
    G4ThreeVector fMomentumDirection;
    G4double      fKineticEnergy;
    G4double      fEnergyDeposition;

public:
    static enum sbScintillatorSet {
        fUpperScintillator,
        fLowerScintillator
    } fScintillatorSet;

public:
    sbScintillatorHit();
    sbScintillatorHit(G4VPhysicalVolume* physicalScintillator);
    sbScintillatorHit(const sbScintillatorHit& rhs);
    ~sbScintillatorHit();
    const sbScintillatorHit& operator=(const sbScintillatorHit& rhs);
    G4bool operator==(const sbScintillatorHit& rhs) const { return this == &rhs; }
    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    const G4int& GetScintillatorID() const { return fScintillatorID; }
    inline const G4String& GetScintillatorName() const;
    const G4double& GetTime() const { return fTime; }
    const G4ThreeVector& GetPosition() const { return fPosition; }
    const G4ThreeVector& GetMomentumDirection() const { return fMomentumDirection; }
    const G4double& GetKineticEnergy() const { return fKineticEnergy; }
    const G4double& GetEnergyDeposition() const { return fEnergyDeposition; }

    void SetScintillatorID(const G4int& scintillatorID) { fScintillatorID = scintillatorID; }
    void SetTime(const G4double& time) { fTime = time; }
    void SetPosition(const G4ThreeVector& momentumDirection) { fMomentumDirection = momentumDirection; }
    void SetMomentumDirection(const G4ThreeVector& position) { fPosition = position; }
    void SetKineticEnergy(const G4double& kineticEnergy) { fKineticEnergy = kineticEnergy; }
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

inline const G4String& sbScintillatorHit::GetScintillatorName() const {
    if (this->fScintillatorID == fUpperScintillator) {
        return gScintillatorsName.first;
    } else {
        return gScintillatorsName.second;
    }
}

#endif

