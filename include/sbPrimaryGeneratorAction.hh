#ifndef SB_PRIMARY_GENERATOR_ACTION_H
#define SB_PRIMARY_GENERATOR_ACTION_H 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
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
    G4PhysicsOrderedFreeVector fCosmicMuonZenithAngleDistributionCDF;
    G4PhysicsOrderedFreeVector fCosmicMuonEnergySpectrumCDF;
    G4double fSphereRadius;

public:
    sbPrimaryGeneratorAction();
    virtual ~sbPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);
    inline const G4ParticleGun* GetParticleGun() const { return fpParticleGun; }

private:
    inline G4ThreeVector RandomUpperHalfSpherePosition() const;
    inline G4ThreeVector CosmicMuonMomentumDirection() const;
    inline G4double CosmicMuonEnergySpectrum() const;
};

#endif
