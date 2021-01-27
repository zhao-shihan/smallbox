#include "sbPrimaryGeneratorAction.hh"

sbPrimaryGeneratorAction::sbPrimaryGeneratorAction() :
    G4VUserPrimaryGeneratorAction(),
    fpParticleGun(new G4ParticleGun(1)),
    fCosmicMuonProperties(
        CreateMapFromCSV<G4double>("./datafiles/cosmicMuonProperties.csv")
    ),
    fCosmicMuonZenithAngleDistributionCDF(
        &fCosmicMuonProperties["thetaDistribution"][0],
        &fCosmicMuonProperties["theta"][0],
        fCosmicMuonProperties["theta"].size()
    ),
    fCosmicMuonEnergySpectrumCDF(
        &fCosmicMuonProperties["energySpectrum"][0],
        &fCosmicMuonProperties["energy"][0],
        fCosmicMuonProperties["energy"].size()
    ),
    fSphereRadius(sbDetectorConstruction::GetInstance()->getWorldRadius() - 1.0) {}

sbPrimaryGeneratorAction::~sbPrimaryGeneratorAction() {
    delete fpParticleGun;
}

void sbPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    if (G4UniformRand() > 0.545) {
        fpParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("mu-"));
    } else {
        fpParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("mu+"));
    }
    fpParticleGun->SetParticlePosition(RandomUpperHalfSpherePosition());
    fpParticleGun->SetParticleEnergy(CosmicMuonEnergySpectrum());
    fpParticleGun->SetParticleMomentumDirection(CosmicMuonMomentumDirection());

    fpParticleGun->GeneratePrimaryVertex(anEvent);
}

constexpr G4double _2_pi = 2.0 * M_PI;

inline G4ThreeVector sbPrimaryGeneratorAction::RandomUpperHalfSpherePosition() const {
    G4double phi = _2_pi * G4UniformRand();
    G4double theta = M_PI_2 * G4UniformRand();
    G4double sin_theta = sin(theta);
    return fSphereRadius * G4ThreeVector(sin_theta * cos(phi), sin_theta * sin(phi), cos(theta));
}

inline G4ThreeVector sbPrimaryGeneratorAction::CosmicMuonMomentumDirection() const {
    G4double phi = _2_pi * G4UniformRand();
    G4double theta = fCosmicMuonZenithAngleDistributionCDF.Value(G4UniformRand());
    G4double sin_theta = sin(theta);
    return G4ThreeVector(sin_theta * cos(phi), sin_theta * sin(phi), -cos(theta));
}

inline G4double sbPrimaryGeneratorAction::CosmicMuonEnergySpectrum() const {
    return fCosmicMuonEnergySpectrumCDF.Value(G4UniformRand()) * GeV;
}

