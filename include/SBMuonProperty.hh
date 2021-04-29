#ifndef SB_MUON_PROPERTY
#define SB_MUON_PROPERTY 1

#include "globals.hh"

struct SBMuonProperty : public std::array<G4double, 5> {
    SBMuonProperty();
    SBMuonProperty(G4double x, G4double y, G4double phi, G4double theta, G4double energy);

    const G4double& GetX() const { return std::get<0>(*this); }
    const G4double& GetY() const { return std::get<1>(*this); }
    const G4double& GetPhi() const { return std::get<2>(*this); }
    const G4double& GetTheta() const { return std::get<3>(*this); }
    const G4double& GetEnergy() const { return std::get<4>(*this); }

    G4double& X() { return std::get<0>(*this); }
    G4double& Y() { return std::get<1>(*this); }
    G4double& Phi() { return std::get<2>(*this); }
    G4double& Theta() { return std::get<3>(*this); }
    G4double& Energy() { return std::get<4>(*this); }
};

#endif

