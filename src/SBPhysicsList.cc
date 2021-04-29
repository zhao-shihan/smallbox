#include "SBPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4SpinDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

SBPhysicsList::SBPhysicsList() : G4VModularPhysicsList() {
    auto opticalParameters = G4OpticalParameters::Instance();
    opticalParameters->SetScintTrackSecondariesFirst(false);
    opticalParameters->SetCerenkovTrackSecondariesFirst(false);
    //Scintillation
    opticalParameters->SetScintYieldFactor(1.0);
    opticalParameters->SetScintExcitationRatio(0.0);
    //Cerenkov light
    opticalParameters->SetCerenkovMaxPhotonsPerStep(INT32_MAX);
    //wave length shifting (WLS)
    opticalParameters->SetWLSTimeProfile("delta");

    //G4SpinDecayPhysics depends on G4DecayPhysics.
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4SpinDecayPhysics());
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4OpticalPhysics());
}

SBPhysicsList::~SBPhysicsList() {}

void SBPhysicsList::SetCuts() {
    G4VUserPhysicsList::SetCuts();
}

