#ifndef SB_PRIMARY_GENERATOR_ACTION_H
#define SB_PRIMARY_GENERATOR_ACTION_H 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "SBAnalysisManager.hh"
#include "SBMuonProperty.hh"
#include "SBDetectorConstruction.hh"

class SBPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
private:
    static std::vector<SBMuonProperty> fMuonEventList;
    
    G4ParticleGun* fMuonPlusGun;
    G4ParticleGun* fMuonMinusGun;

    SBAnalysisManager* fAnalysisManager;

    const SBDetectorConstruction* fDetectorConstruction;

public:
    SBPrimaryGeneratorAction();
    virtual ~SBPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);

    static void SetMuonEventList(const std::vector<SBMuonProperty>& eventList) { fMuonEventList = eventList; }

private:
    G4double EnergySpectrum(G4double energy, G4double theta) const;
    void FindEnergyAndTheta(G4double& energy, G4double& theta) const;
};

#endif

