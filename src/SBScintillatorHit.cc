#include "SBScintillatorHit.hh"
#include "SBDetectorConstruction.hh"

G4ThreadLocal G4Allocator<SBScintillatorHit>* SBScintillatorHitAllocator = nullptr;

SBScintillatorHit::SBScintillatorHit() :
    G4VHit(),
    // fScintillatorID(-1),
    // fTime(0.0),
    fPosition(0.0),
    fMomentum(0.0),
    fKineticEnergy(0.0)
    // fEnergyDeposition(0.0),
    // fParticleDefinition(nullptr)
{}

SBScintillatorHit::SBScintillatorHit
(const G4ThreeVector& position, const G4ThreeVector& momentum, const G4double& energy) :
    G4VHit(),
    // fScintillatorID(-1),
    // fTime(0.0),
    fPosition(position),
    fMomentum(momentum),
    fKineticEnergy(energy)
    // fEnergyDeposition(0.0),
    // fParticleDefinition(nullptr)
{}

SBScintillatorHit::SBScintillatorHit(const SBScintillatorHit& rhs) :
    G4VHit(),
    // fScintillatorID(rhs.fScintillatorID),
    // fTime(rhs.fTime),
    fPosition(rhs.fPosition),
    fMomentum(rhs.fMomentum),
    fKineticEnergy(rhs.fKineticEnergy)
    // fEnergyDeposition(rhs.fEnergyDeposition),
    // fParticleDefinition(rhs.fParticleDefinition)
{}

SBScintillatorHit::~SBScintillatorHit() {}

const SBScintillatorHit& SBScintillatorHit::operator=(const SBScintillatorHit& rhs) {
    if (&rhs != this) {
        // this->fScintillatorID = rhs.fScintillatorID;
        // this->fTime = rhs.fTime;
        this->fPosition = rhs.fPosition;
        this->fMomentum = rhs.fMomentum;
        this->fKineticEnergy = rhs.fKineticEnergy;
        // this->fEnergyDeposition = rhs.fEnergyDeposition;
        // this->fParticleDefinition = rhs.fParticleDefinition;
    }
    return *this;
}

