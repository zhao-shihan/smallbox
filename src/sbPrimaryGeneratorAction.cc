#include "sbPrimaryGeneratorAction.hh"

sbPrimaryGeneratorAction::sbPrimaryGeneratorAction() :
    G4VUserPrimaryGeneratorAction() {
    G4int n_particle = 1;
    pParticleGun = new G4ParticleGun(n_particle);
}

sbPrimaryGeneratorAction::~sbPrimaryGeneratorAction() {
    delete pParticleGun;
}

void sbPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    if (G4UniformRand() > 0.545) {
        pParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("mu-"));
    } else {
        pParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("mu+"));
    }
    pParticleGun->SetParticlePosition(RandomUpperHalfSpherePosition());
    pParticleGun->SetParticleEnergy(10.0 * MeV);
    pParticleGun->SetParticleMomentumDirection(CosmicMuonMomentumDirection());

    pParticleGun->GeneratePrimaryVertex(anEvent);
}

inline G4ThreeVector sbPrimaryGeneratorAction::RandomUpperHalfSpherePosition() const {
    const G4double radius = 1.0 * m;
    const G4double phi = M_PI * (2.0 * G4UniformRand() - 1);
    G4double theta = M_PI_2 * G4UniformRand();
    G4double sin_theta = sin(theta);
    G4double x0 = radius * sin_theta * cos(phi);
    G4double y0 = radius * sin_theta * sin(phi);
    G4double z0 = radius * cos(theta);
    return G4ThreeVector(x0, y0, z0);
}

inline G4ThreeVector sbPrimaryGeneratorAction::CosmicMuonMomentumDirection() const {
    G4double theta, y, phi;
    do {
        theta = M_PI_2 * G4UniformRand();
        y = M_2_PI * G4UniformRand();
    } while (y > this->CosmicMuonZenithAngleDistribution(theta));
    phi = M_PI * (2 * G4UniformRand() - 1);
    G4double sin_theta = sin(theta);
    return G4ThreeVector(sin_theta * cos(phi), sin_theta * sin(phi), -cos(theta));
}

inline G4double sbPrimaryGeneratorAction::CosmicMuonZenithAngleDistribution(const G4double& theta) const {
    G4double sin_theta = sin(theta);
    return M_2_PI * sin_theta * sin_theta;
}

