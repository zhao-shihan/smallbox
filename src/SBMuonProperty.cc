#include "SBMuonProperty.hh"

SBMuonProperty::SBMuonProperty() :
    std::array<G4double, 5>({ 0.0, 0.0, 0.0, 0.0, 0.0 }) {}

SBMuonProperty::SBMuonProperty(G4double x, G4double y, G4double phi, G4double theta, G4double energy) :
    std::array<G4double, 5>({ x, y, phi, theta, energy }) {}