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
    G4ParticleGun* fParticleGun;

public:
    sbPrimaryGeneratorAction();
    virtual ~sbPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);
    inline const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

private:
    G4double EnergySpectrum(G4double E_GeV, G4double theta) const;
    void FindEnergyAndTheta(G4double& energy, G4double& theta) const;

    void SetMuonProperties() const;
};

#endif

