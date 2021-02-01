#include "sbPrimaryGeneratorAction.hh"

sbPrimaryGeneratorAction::sbPrimaryGeneratorAction() :
    G4VUserPrimaryGeneratorAction(),
    fpParticleGun(new G4ParticleGun(1)),
    fCosmicMuonProperties(
        CreateMapFromCSV<G4double>("./datafiles/cosmicMuonProperties.csv")
    ),
    fCosmicMuonZenithDistributionCDFinv(
        &fCosmicMuonProperties["thetaDistribution"][0],
        &fCosmicMuonProperties["theta"][0],
        fCosmicMuonProperties["theta"].size()
    ),
    fCosmicMuonEnergySpectrumCDFinv(
        &fCosmicMuonProperties["energySpectrum"][0],
        &fCosmicMuonProperties["energy"][0],
        fCosmicMuonProperties["energy"].size()
    ) {}

sbPrimaryGeneratorAction::~sbPrimaryGeneratorAction() {
    delete fpParticleGun;
}

void sbPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    if (G4UniformRand() > 0.545) {
        fpParticleGun->SetParticleDefinition(G4MuonMinus::Definition());
    } else {
        fpParticleGun->SetParticleDefinition(G4MuonPlus::Definition());
    }
    
    auto posAndDir = MuonPositionAndDirection();
    fpParticleGun->SetParticlePosition(posAndDir.first);
    fpParticleGun->SetParticleMomentumDirection(posAndDir.second);
    
    fpParticleGun->SetParticleEnergy(CosmicMuonEnergySpectrum());

    fpParticleGun->GeneratePrimaryVertex(anEvent);
}

constexpr G4double _2_pi = 2.0 * M_PI;

inline std::pair<G4ThreeVector, G4ThreeVector>
sbPrimaryGeneratorAction::MuonPositionAndDirection() const {
    auto sphereCentre = G4ThreeVector(
        2.0 * G4UniformRand() - 1.0,
        2.0 * G4UniformRand() - 1.0,
        0.0
    ) * gEffectiveRange;

    G4double phi = _2_pi * G4UniformRand();
    G4double theta = fCosmicMuonZenithDistributionCDFinv.Value(G4UniformRand());
    G4double sinTheta = sin(theta);
    auto relativePositionVec = G4ThreeVector(
        sinTheta * cos(phi),
        sinTheta * sin(phi),
        cos(theta)
    ) * gSphereRadius;

    auto directionVec = -relativePositionVec;

    return std::pair<G4ThreeVector, G4ThreeVector>(relativePositionVec + sphereCentre, directionVec);
}

