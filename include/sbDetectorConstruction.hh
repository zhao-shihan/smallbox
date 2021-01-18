#ifndef SB_DETECTOR_CONSTRUCTION_H
#define SB_DETECTOR_CONSTRUCTION_H 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4SDManager.hh"

#include "sbTrackerSD.hh"
#include "sbSettings.hh"
#include "sbXYlist.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class sbDetectorConstruction : public G4VUserDetectorConstruction {
public:
    static sbDetectorConstruction* GetInstance() {
        if (instance_ == nullptr) { instance_ = new sbDetectorConstruction(); }
        return instance_;
    }
    sbDetectorConstruction(const sbDetectorConstruction&) = delete;
    sbDetectorConstruction operator=(const sbDetectorConstruction&) = delete;
private:
    static sbDetectorConstruction* instance_;
    sbDetectorConstruction();
    virtual ~sbDetectorConstruction() {}

private:
    G4NistManager* nist;
    //
    // world's size, name, and materials

    G4double world_radius_;
    G4String world_name_;
    G4String world_material_name_;
    //
    // scintillators' size, name, and materials

    G4double scintillator_half_size_[3];
    G4double scintillator_centre_distance_;
    G4String scintillator_1_name_;
    G4String scintillator_2_name_;
    G4String sensitive_detector_name_;
    G4String scintillator_material_name_;
    //
    // aluminum foils' size, name, and materials

    G4double al_foil_thickness_;
    G4double al_foil_hole_half_width_;
    G4double al_foil_scintillator_gap_;
    G4String al_foil_1_name_;
    G4String al_foil_2_name_;
    G4String al_foil_material_name_;

public:
    virtual G4VPhysicalVolume* Construct();
    G4double get_world_radius() const { return world_radius_; }
private:
    void SetScintillatorsAsSensitiveDetector(const G4String& scintillator_name,
        const G4String& sensitive_detector_name);
    //
    // World material optical properties setting.
    // -> Refraction index
    void SetWorldOpticalProperties(G4Material*& world_material) const;
    //
    // Scintillator material optical properties setting.
    // -> Absorption length
    // -> Refraction index
    // -> Reemission probablity
    // -> Rayleigh scattering
    // -> Fast/Slow light component
    // -> Scintillation yield
    // -> Resolution scale
    // -> Fast time constant
    // -> Slow time constant
    // -> Yield ratio
    // -> Alpha fast time constant
    // -> Alpha Slow time constant
    // -> Alpha yield ratio
    // -> Neutron fast time constant
    // -> Neutron Slow time constant
    // -> Neutron yield ratio
    // -> Reemission fast time constant
    // -> Reemission Slow time constant
    // -> Reemission yield ratio
    void SetScintillatorOpticalProperties(G4Material*& scintillator_material) const;
    // 
    // Aluminum foil surface optical properties setting.
    // -> Reflectivity
    void SetAlFoilOpticalProperties(G4OpticalSurface*& al_foil_optical_surface) const;
};

#endif

