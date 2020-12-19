#ifndef SB_DETECTOR_CONSTRUCTION_H
#define SB_DETECTOR_CONSTRUCTION_H 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class sbDetectorConstruction : public G4VUserDetectorConstruction {
public:
    sbDetectorConstruction();
    virtual ~sbDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    void ConstructScintillatorOpticalSurface(G4Material*& scintillator_material, G4OpticalSurface*&);
    void ConstructAlFoilOpticalSurface(G4Material*&, G4OpticalSurface*& surface);
};

#endif

