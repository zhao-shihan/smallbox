#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalPhoton.hh"

#include "sbSteppingAction.hh"
#include "sbEventAction.hh"
#include "sbDetectorConstruction.hh"

sbSteppingAction::sbSteppingAction(sbEventAction* eventAction) :
    G4UserSteppingAction(), fEventAction(eventAction) {}

sbSteppingAction::~sbSteppingAction() {}

void sbSteppingAction::UserSteppingAction(const G4Step* /* step */) {}

