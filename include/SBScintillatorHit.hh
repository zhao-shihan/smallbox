#ifndef SB_SCINTILLATOR_HIT_H
#define SB_SCINTILLATOR_HIT_H 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"

class SBScintillatorHit : public G4VHit {
private:
    // G4int                       fScintillatorID;
    // G4double                    fTime;
    G4ThreeVector               fPosition;
    G4ThreeVector               fMomentum;
    G4double                    fKineticEnergy;
    // G4double                    fEnergyDeposition;
    // const G4ParticleDefinition* fParticleDefinition;

public:
    SBScintillatorHit();
    SBScintillatorHit(const G4ThreeVector& position, const G4ThreeVector& momentum, const G4double& energy);
    SBScintillatorHit(const SBScintillatorHit& rhs);
    ~SBScintillatorHit();
    const SBScintillatorHit& operator=(const SBScintillatorHit& rhs);
    G4bool operator==(const SBScintillatorHit& rhs) const { return this == &rhs; }
    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    // const G4int& GetScintillatorID() const { return fScintillatorID; }
    // const G4double& GetTime() const { return fTime; }
    const G4ThreeVector& GetPosition() const { return fPosition; }
    const G4ThreeVector& GetMomentum() const { return fMomentum; }
    const G4double& GetKineticEnergy() const { return fKineticEnergy; }
    // const G4double& GetEnergyDeposition() const { return fEnergyDeposition; }
    // const G4ParticleDefinition* GetParticleDefinition() const { return fParticleDefinition; }

    // void SetScintillatorID(const G4int& scintillatorID) { fScintillatorID = scintillatorID; }
    // void SetTime(const G4double& time) { fTime = time; }
    void SetPosition(const G4ThreeVector& position) { fPosition = position; }
    void SetMomentum(const G4ThreeVector& momentum) { fMomentum = momentum; }
    void SetKineticEnergy(const G4double& kineticEnergy) { fKineticEnergy = kineticEnergy; }
    // void SetEnergyDeposition(const G4double& energyDeposition) { fEnergyDeposition = energyDeposition; }
    // void SetParticleDefinition(const G4ParticleDefinition* particleDefinition) {
    //     fParticleDefinition = particleDefinition;
    // }
};

typedef G4THitsCollection<SBScintillatorHit> SBScintillatorHitsCollection;

extern G4ThreadLocal G4Allocator<SBScintillatorHit>* SBScintillatorHitAllocator;

inline void* SBScintillatorHit::operator new(size_t) {
    if (!SBScintillatorHitAllocator) {
        SBScintillatorHitAllocator = new G4Allocator<SBScintillatorHit>();
    }
    return (void*)SBScintillatorHitAllocator->MallocSingle();
}

inline void SBScintillatorHit::operator delete(void* aHit) {
    SBScintillatorHitAllocator->FreeSingle(static_cast<SBScintillatorHit*>(aHit));
}

#endif

