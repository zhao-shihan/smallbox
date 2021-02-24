#include "sbScintillatorHit.hh"
#include "sbDetectorConstruction.hh"

G4ThreadLocal G4Allocator<sbScintillatorHit>* sbScintillatorHitAllocator = nullptr;

sbScintillatorHit::sbScintillatorHit() :
    G4VHit(),
    fScintillatorID(-1),
    fTime(0.0),
    fPosition(0.0),
    fMomentumDirection(0.0),
    fKineticEnergy(0.0),
    fEnergyDeposition(0.0),
    fParticleDefinition(nullptr) {}

sbScintillatorHit::sbScintillatorHit(G4VPhysicalVolume* physicalScintillator) :
    G4VHit(),
    fTime(0.0),
    fPosition(0.0),
    fMomentumDirection(0.0),
    fKineticEnergy(0.0),
    fEnergyDeposition(0.0),
    fParticleDefinition(nullptr) {
    auto sbDC = sbDetectorConstruction::GetsbDCInstance();
    if (physicalScintillator == sbDC->GetPhysicalScintillators().first) {
        fScintillatorID = fUpperScintillator;
    } else if (physicalScintillator == sbDC->GetPhysicalScintillators().second) {
        fScintillatorID = fLowerScintillator;
    } else {
        G4ExceptionDescription exceptout;
        exceptout << "The scintillator physical volume not found." << G4endl;
        exceptout << "Maybe you have changed the detector construction?" << G4endl;
        G4Exception(
            "sbScintillatorHit::sbScintillatorHit(G4VPhysicalVolume* physicalScintillator)",
            "ScintillatorNotFound",
            FatalException,
            exceptout
        );
    }
}

sbScintillatorHit::sbScintillatorHit(const sbScintillatorHit& rhs) :
    G4VHit(),
    fScintillatorID(rhs.fScintillatorID),
    fTime(rhs.fTime),
    fPosition(rhs.fPosition),
    fMomentumDirection(rhs.fMomentumDirection),
    fKineticEnergy(rhs.fKineticEnergy),
    fEnergyDeposition(rhs.fEnergyDeposition),
    fParticleDefinition(rhs.fParticleDefinition) {}

sbScintillatorHit::~sbScintillatorHit() {}

const sbScintillatorHit& sbScintillatorHit::operator=(const sbScintillatorHit& rhs) {
    if (&rhs != this) {
        this->fScintillatorID = rhs.fScintillatorID;
        this->fTime = rhs.fTime;
        this->fPosition = rhs.fPosition;
        this->fMomentumDirection = rhs.fMomentumDirection;
        this->fKineticEnergy = rhs.fKineticEnergy;
        this->fEnergyDeposition = rhs.fEnergyDeposition;
        this->fParticleDefinition = rhs.fParticleDefinition;
    }
    return *this;
}

