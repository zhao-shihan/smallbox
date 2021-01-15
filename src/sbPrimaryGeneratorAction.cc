#include "sbPrimaryGeneratorAction.hh"

sbPrimaryGeneratorAction::sbPrimaryGeneratorAction() :
    G4VUserPrimaryGeneratorAction(),
    particle_gun(new G4ParticleGun(1)),
    cosmic_muon_zenith_angle_distribution(
        new sbInterpolatingFunction("data/cosmic_muon_zenith_angle_distribution_CDF_inverse.csv", 100)) {}

sbPrimaryGeneratorAction::~sbPrimaryGeneratorAction() {
    delete particle_gun;
    delete cosmic_muon_zenith_angle_distribution;
}

void sbPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    if (G4UniformRand() > 0.545) {
        particle_gun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("mu-"));
    } else {
        particle_gun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("mu+"));
    }
    particle_gun->SetParticlePosition(RandomUpperHalfSpherePosition());
    particle_gun->SetParticleEnergy(10.0 * MeV);
    particle_gun->SetParticleMomentumDirection(CosmicMuonMomentumDirection());

    particle_gun->GeneratePrimaryVertex(anEvent);
}

inline G4ThreeVector sbPrimaryGeneratorAction::RandomUpperHalfSpherePosition() const {
    const G4double radius = 1.0 * m;
    G4double phi = M_PI * (2.0 * G4UniformRand() - 1);
    G4double theta = M_PI_2 * G4UniformRand();
    G4double sin_theta = sin(theta);
    return G4ThreeVector(radius * sin_theta * cos(phi), radius * sin_theta * sin(phi), radius * cos(theta));
}

inline G4ThreeVector sbPrimaryGeneratorAction::CosmicMuonMomentumDirection() const {
    G4double phi = M_PI * (2 * G4UniformRand() - 1);
    G4double theta = (*cosmic_muon_zenith_angle_distribution)(G4UniformRand());
    G4double sin_theta = sin(theta);
    return G4ThreeVector(sin_theta * cos(phi), sin_theta * sin(phi), -cos(theta));
}

