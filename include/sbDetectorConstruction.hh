#pragma once
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
#include "G4OpticalSurface.hh"
#include "G4SDManager.hh"

#include "sbTrackerSD.hh"
#include "sbFunctionSwitch.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class sbDetectorConstruction : public G4VUserDetectorConstruction {
public:
    sbDetectorConstruction();
    virtual ~sbDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    void ConstructScintillatorsAsSensitiveDetector(const G4String& scintillator_name,
        const G4String& sensitive_detector_name);
    void ConstructScintillatorOptics(G4Material*& scintillator_material, G4OpticalSurface*&);
    void ConstructAlFoilOptics(G4Material*&, G4OpticalSurface*& surface);
};

#endif

