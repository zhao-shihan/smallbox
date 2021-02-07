#ifndef SB_PRIMARY_GENERATOR_ACTION_H
#define SB_PRIMARY_GENERATOR_ACTION_H 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Sphere.hh"
#include "Randomize.hh"

#include "sbDetectorConstruction.hh"
#include "CreateMapFromCSV.hh"

class sbDetectorConstruction;
class G4ParticleGun;
class G4Event;
class G4Sphere;

class sbPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
private:
    G4ParticleGun* fpParticleGun;
    std::map<std::string, G4PVDataVector> fCosmicMuonProperties;
    G4PhysicsOrderedFreeVector fCosmicMuonZenithDistributionCDFinv;
    G4PhysicsOrderedFreeVector fCosmicMuonEnergySpectrumCDFinv;

public:
    sbPrimaryGeneratorAction();
    virtual ~sbPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);
    inline const G4ParticleGun* GetParticleGun() const { return fpParticleGun; }

private:
    inline std::pair<G4ThreeVector, G4ThreeVector> MuonPositionAndDirection() const;
    inline G4double CosmicMuonEnergySpectrum() const {
        return fCosmicMuonEnergySpectrumCDFinv.Value(G4UniformRand()) * GeV;
    }
};

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

#endif
