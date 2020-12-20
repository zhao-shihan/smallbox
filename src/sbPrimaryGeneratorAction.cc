#include "sbPrimaryGeneratorAction.hh"

sbPrimaryGeneratorAction::sbPrimaryGeneratorAction() :
    G4VUserPrimaryGeneratorAction(),
    pParticleGun(0) {
    G4int n_particle = 1;
    pParticleGun = new G4ParticleGun(n_particle);
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    if (G4UniformRand() > 0.545) {
        particleName = "mu-";
    } else {
        particleName = "mu+";
    }
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
    pParticleGun->SetParticleDefinition(particle);
    pParticleGun->SetParticleMomentumDirection(CosmicMuonMomentumDirection());
    pParticleGun->SetParticleEnergy(10 * MeV);
}

sbPrimaryGeneratorAction::~sbPrimaryGeneratorAction() {
    delete pParticleGun;
}

void sbPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    G4double radius = 1.0 * m;
    /*     if (!solid_world_) {
            G4LogicalVolume* logical_world = G4LogicalVolumeStore::GetInstance()->GetVolume("world");
            if (logical_world) {
                solid_world_ = dynamic_cast<G4Sphere*>(logical_world->GetSolid());
            } else {
                G4ExceptionDescription msg;
                msg << "Message from sbPrimaryGeneratorAction::GeneratePrimaries(G4Event*):" << std::endl;
                msg << "solid world not found! Check geometry." << std::endl;
                msg << "Particles will generate @ R=1m ." << std::endl;
                G4Exception("B1PrimaryGeneratorAction::GeneratePrimaries(G4Event*)", "MyCode0001", JustWarning, msg);
            }
        } else {
            radius = solid_world_->GetRmax();
        } */

    G4double phi = M_PI * (2 * G4UniformRand() - 1);
    G4double theta = M_PI_2 * G4UniformRand();
    G4double x0 = radius * sin(theta) * cos(phi);
    G4double y0 = radius * sin(theta) * sin(phi);
    G4double z0 = radius * cos(theta);

    pParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    pParticleGun->GeneratePrimaryVertex(anEvent);
}

inline G4ThreeVector sbPrimaryGeneratorAction::CosmicMuonMomentumDirection() {
    G4double theta, y, phi;
    do {
        theta = M_PI_2 * G4UniformRand();
        y = M_2_PI * G4UniformRand();
    } while (y > this->CosmicMuonZenithAngleDistribution(theta));
    phi = M_PI * (2 * G4UniformRand() - 1);
    return G4ThreeVector(sin(theta) * cos(phi), sin(theta) * sin(phi), -cos(theta));
}

inline G4double sbPrimaryGeneratorAction::CosmicMuonZenithAngleDistribution(G4double theta) {
    G4double sin_theta = sin(theta);
    return M_2_PI * sin_theta * sin_theta;
}

