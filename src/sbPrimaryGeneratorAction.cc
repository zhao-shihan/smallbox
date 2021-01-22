#include "sbPrimaryGeneratorAction.hh"

sbPrimaryGeneratorAction::sbPrimaryGeneratorAction() :
    G4VUserPrimaryGeneratorAction(),
    particle_gun_(new G4ParticleGun(1)),
    cosmic_muon_zenith_angle_distribution_("cosmic_muon_zenith_angle_distribution_CDF_inverse.csv", 100),
    cosmic_muon_energy_spectrum_("cosmic_muon_energy_spectrum_CDF_inverse.csv", 1000) {}

sbPrimaryGeneratorAction::~sbPrimaryGeneratorAction() {
    delete particle_gun_;
}

void sbPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    if (G4UniformRand() > 0.545) {
        particle_gun_->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("mu-"));
    } else {
        particle_gun_->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("mu+"));
    }
    particle_gun_->SetParticlePosition(RandomUpperHalfSpherePosition());
    particle_gun_->SetParticleEnergy(CosmicMuonEnergySpectrum());
    particle_gun_->SetParticleMomentumDirection(CosmicMuonMomentumDirection());

    particle_gun_->GeneratePrimaryVertex(anEvent);
}

constexpr G4double _2_pi = 2.0 * M_PI;

inline G4ThreeVector sbPrimaryGeneratorAction::RandomUpperHalfSpherePosition() const {
    const G4double radius = sbDetectorConstruction::GetInstance()->get_world_radius() - 1.0;
    G4double phi = _2_pi * G4UniformRand();
    G4double theta = M_PI_2 * G4UniformRand();
    G4double sin_theta = sin(theta);
    return G4ThreeVector(radius * sin_theta * cos(phi), radius * sin_theta * sin(phi), radius * cos(theta));
}

inline G4ThreeVector sbPrimaryGeneratorAction::CosmicMuonMomentumDirection() const {
    G4double phi = _2_pi * G4UniformRand();
    G4double theta = cosmic_muon_zenith_angle_distribution_(G4UniformRand());
    G4double sin_theta = sin(theta);
    return G4ThreeVector(sin_theta * cos(phi), sin_theta * sin(phi), -cos(theta));
}

inline G4double sbPrimaryGeneratorAction::CosmicMuonEnergySpectrum() const {
    return cosmic_muon_energy_spectrum_(G4UniformRand()) * GeV;
}

