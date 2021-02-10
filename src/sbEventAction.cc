#include "sbEventAction.hh"
#include "sbGlobal.hh"

sbEventAction::sbEventAction(sbRunAction* runAction) :
    G4UserEventAction(),
    fRunAction(runAction) {}

sbEventAction::~sbEventAction() {}

void sbEventAction::BeginOfEventAction(const G4Event*) {}

void sbEventAction::EndOfEventAction(const G4Event*) {}

