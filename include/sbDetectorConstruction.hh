#ifndef SB_DETECTOR_CONSTRUCTION_H
#define SB_DETECTOR_CONSTRUCTION_H 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4SDManager.hh"

#include "sbGlobal.hh"
#include "sbConfigs.hh"
#include "CreateMapFromCSV.hh"
#include "sbScintillatorSD.hh"

class sbDetectorConstruction : public G4VUserDetectorConstruction {
public:
    static sbDetectorConstruction* GetsbDCInstance() {
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
    // Scintillator logical volume
    // Note: use for registering sensitive detector in ConstructSDandField().
    G4LogicalVolume* fLogicalScintillator;
    
public:
    virtual G4VPhysicalVolume* Construct();

private:
    virtual void ConstructSDandField();
    //
    // World material optical properties setting.
    // -> Refraction index
    void SetWorldOpticalProperties(G4Material* worldMaterial) const;
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
    void SetScintillatorOpticalProperties(G4Material* scintillatorMaterial) const;
    // 
    // Aluminum foil surface optical properties setting.
    // -> Reflectivity
    void SetAlFoilOpticalProperties(G4OpticalSurface* alFoilOpticalSurface) const;
};

#endif

