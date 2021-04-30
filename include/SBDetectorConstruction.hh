#ifndef SB_DETECTOR_CONSTRUCTION_H
#define SB_DETECTOR_CONSTRUCTION_H 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"

#include "CSVHandler.hh"
#include "SBScintillatorSD.hh"
#include "SBSiPMSD.hh"
#include "SBConfigs.hh"

class SBDetectorConstruction : public G4VUserDetectorConstruction {
public:

    // World

    const G4String fWorldName;

    // Scintillator

    const G4double fScintillatorHalfSize[3];
    const G4String fScintillatorName;

    // Acrylic light guide

    const G4double fAcrylicLightGuideSiPMSideHalfSize[2];
    const G4double fAcrylicLightGuideLength;
    const G4String fAcrylicName;

    // Gap between acrylic and scintillator

    const G4double fAcrylicScintillatorGap;
    const G4String fAcrylicScintillatorGapName;

    // Gap between cover and scintillator

    const G4double fCoverScintillatorGap;
    const G4String fCoverScintillatorGapName;

    // Aluminum foil cover

    const G4double fCoverThickness;
    const G4double fCoverHoleHalfWidth;
    const G4String fCoverName;

    // SiPM

    const G4int fSiPMCount;
    const G4double fSiPMHalfSize[3];
    const G4double fSiPMScintillatorGap;
    const G4String fSiPMName;

    // Silicone Oil (Gap between SiPM and acrylic)

    const G4double fSiliconeOilHalfWidth;
    const G4double fSiliconeOilThickness;
    const G4String fSiliconeOilName;

    // PCB

    const G4double fPCBHalfSize[3];
    const G4String fPCBName;

    // detector distance

    const G4double fDetectorDistance;

    // other

    const G4String fTempName;

private:
    G4double fWorldZMax;
    //
    // Scintillator logical volume
    // Note: use for registering sensitive detector in ConstructSDandField().
    G4LogicalVolume* fLogicalScintillator;
    //
    // SiPM logical volume
    // Note: use for registering sensitive detector in ConstructSDandField().
    G4LogicalVolume* fLogicalSiPM;
    G4int fDetectorPartCount;

public:
    virtual ~SBDetectorConstruction();
    SBDetectorConstruction();
    virtual G4VPhysicalVolume* Construct();

    const G4double& GetWorldZMax() const { return fWorldZMax; }
    const G4int& GetDetectorPartCount() const { return fDetectorPartCount; }

private:
    virtual void ConstructSDandField();
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
};

#endif

