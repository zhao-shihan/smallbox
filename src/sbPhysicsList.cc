#include "sbPhysicsList.hh"

sbPhysicsList::sbPhysicsList() : G4VModularPhysicsList() {
    RegisterPhysics(OpticalPhysics_init());

    //G4SpinDecayPhysics depends on G4DecayPhysics.
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4SpinDecayPhysics());
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4EmPenelopePhysics());
    RegisterPhysics(new G4StepLimiterPhysics());
}

sbPhysicsList::~sbPhysicsList() {}

void sbPhysicsList::SetCuts() {
    G4VUserPhysicsList::SetCuts();
}

G4OpticalPhysics* sbPhysicsList::OpticalPhysics_init() {
    auto pOptics = new G4OpticalPhysics;

    pOptics->SetTrackSecondariesFirst(kScintillation, false);
    pOptics->SetTrackSecondariesFirst(kCerenkov, false);
    pOptics->SetTrackSecondariesFirst(kWLS, false);

    //Scintillation
    pOptics->SetScintillationYieldFactor(1.);
    pOptics->SetScintillationExcitationRatio(0.);

    //Cerenkov light
    pOptics->SetMaxNumPhotonsPerStep(50);

    //wave length shifting (WLS)
    pOptics->SetWLSTimeProfile("delta");

    return pOptics;
}

