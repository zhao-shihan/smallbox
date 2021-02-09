#ifndef SB_DETECTOR_CONSTRUCTION_H
#define SB_DETECTOR_CONSTRUCTION_H 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4SDManager.hh"

#include "sbGlobal.hh"
#include "sbConfigs.hh"
#include "CreateMapFromCSV.hh"
#include "sbScintillatorSD.hh"
#include "sbSiPMSD.hh"

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
    //
    // Scintillators physical volume
    // Note: use for identifying scintillator in sbScintillatorHit.
    std::pair<G4VPhysicalVolume*, G4VPhysicalVolume*> fPhysicalScintillators;

    //
    // SiPM logical volume
    // Note: use for registering sensitive detector in ConstructSDandField().
    G4LogicalVolume* fLogicalSiPM;
    //
    // SiPMs physical volume
    // Note: use for identifying SiPM in sbSiPMHit.
    std::pair<G4VPhysicalVolume*, G4VPhysicalVolume*> fPhysicalSiPMs;

public:
    virtual G4VPhysicalVolume* Construct();

    const std::pair<G4VPhysicalVolume*, G4VPhysicalVolume*>& GetPhysicalScintillators() const {
        return fPhysicalScintillators;
    }
    const std::pair<G4VPhysicalVolume*, G4VPhysicalVolume*>& GetPhysicalSiPMs() const {
        return fPhysicalSiPMs;
    }

private:
    virtual void ConstructSDandField();
    //
    // World material optical properties setting.
    // -> Refraction index
    void SetWorldMaterialProperties(G4Material* worldMaterial) const;
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
    void SetScintillatorMaterialProperties(G4Material* scintillatorMaterial) const;
    // 
    // Aluminum foil surface optical properties setting.
    // -> Reflectivity
    void SetAlFoilSurfaceProperties(G4OpticalSurface* alFoilOpticalSurface) const;
    // 
    // SiPM surface optical properties setting.
    // -> Refraction index
    // Note: for optical photon hit.
    void SetSiPMMaterialProperties(G4Material* SiPMMaterial) const;
};

#endif

