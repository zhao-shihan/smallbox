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

#include "sbConfigs.hh"
#include "CreateMapFromCSV.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class sbDetectorConstruction : public G4VUserDetectorConstruction {
public:
    static sbDetectorConstruction* GetInstance() {
        if (!sbDCInstance) { sbDCInstance = new sbDetectorConstruction(); }
        return sbDCInstance;
    }
    sbDetectorConstruction(const sbDetectorConstruction&) = delete;
    sbDetectorConstruction& operator=(const sbDetectorConstruction&) = delete;
private:
    static sbDetectorConstruction* sbDCInstance;
    sbDetectorConstruction();
    virtual ~sbDetectorConstruction() {}

private:
    //
    // world's size, name, and materials

    G4double fWorldRadius;
    G4String fWorldName;
    G4String fWorldMaterialName;
    //
    // scintillators' size, name, and materials

    G4double fScintillatorHalfSize[3];
    G4double fScintillatorDistance;
    G4String fScintillator1Name;
    G4String fScintillator2Name;
    G4String fSensitiveDetectorName;
    G4String fScintillatorMaterialName;
    //
    // aluminum foils' size, name, and materials

    G4double fAlFoilThickness;
    G4double fAlFoilHoleHalfWidth;
    G4double fAlFoilScintillatorGap;
    G4String fAlFoil1Name;
    G4String fAlFoil2Name;
    G4String fAlFoilMaterialName;

public:
    virtual G4VPhysicalVolume* Construct();
    G4double getWorldRadius() const { return fWorldRadius; }
private:
    // void ConstructSDandField(G4LogicalVolume* logical_scintillator,
    //     const G4String& sensitive_detector_name);
    //
    // World material optical properties setting.
    // -> Refraction index
    void SetWorldOpticalProperties(G4Material* world_material) const;
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
    void SetScintillatorOpticalProperties(G4Material* scintillator_material) const;
    // 
    // Aluminum foil surface optical properties setting.
    // -> Reflectivity
    void SetAlFoilOpticalProperties(G4OpticalSurface* al_foil_optical_surface) const;
};

#endif

