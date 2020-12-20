#ifndef SB_PRIMARY_GENERATOR_ACTION_H
#define SB_PRIMARY_GENERATOR_ACTION_H 1

#include "sbDetectorConstruction.hh"

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

class sbDetectorConstruction;
class G4ParticleGun;
class G4Event;
class G4Sphere;

class sbPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    sbPrimaryGeneratorAction();
    virtual ~sbPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);
    inline const G4ParticleGun* GetParticleGun() const { return pParticleGun; }

private:
    inline G4ThreeVector CosmicMuonMomentumDirection();
    inline G4double CosmicMuonZenithAngleDistribution(G4double theta);
    G4ParticleGun* pParticleGun; // pointer a to G4 gun class
    G4Sphere* solid_world_;
};

#endif
