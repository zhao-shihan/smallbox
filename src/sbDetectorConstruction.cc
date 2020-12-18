#include "sbDetectorConstruction.hh"

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

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();

    const G4double scintillator_centre_distance = 1.0 * cm;
    const G4double scintillator_half_size[3]{ 5.0 * cm, 5.0 * cm, 0.5 * cm };
    const G4double al_foil_thickness = 0.5 * mm;
    const G4double world_radius = 1 * m;

    const G4String world_name("World");
    const G4String world_material_name("G4_AIR");

    const G4String scintillator_1_name("Scintillator_1");
    const G4ThreeVector scintillator_1_position(0.0 * cm, 0.0 * cm,
        scintillator_centre_distance / 2.0 + scintillator_half_size[2]);
    const G4String scintillator_2_name("Scintillator_2");
    const G4ThreeVector scintillator_2_position(0.0 * cm, 0.0 * cm,
        -scintillator_centre_distance / 2.0 - scintillator_half_size[2]);
    const G4String scintillator_material_name("Plastic_scintillator");

    const G4String al_foil_1_name("Al_foil_1");
    const G4String al_foil_2_name("Al_foil_2");
    const G4String al_foil_material_name("G4_Al");

    const G4bool check_overlaps = true;

    G4Material* world_material = nist->FindOrBuildMaterial(world_material_name);

    G4Material* scintillator_material = new G4Material(scintillator_material_name, 1.032 * g / cm3, 2, kStateSolid);
    G4Element* element_H = new G4Element("Hydrogen", "H", 1.0, 1.008 * g / mole);
    G4Element* element_C = new G4Element("Carbon", "C", 6.0, 12.01 * g / mole);
    scintillator_material->AddElement(element_C, 9);
    scintillator_material->AddElement(element_H, 10);

    G4Material* al_foil_material = nist->FindOrBuildMaterial(al_foil_material_name);

    // World
    //
    G4Sphere* big_sphere = new G4Sphere(
        "big_sphere",
        0.0,
        world_radius,
        0.0,
        360 * deg,
        0.0,
        360 * deg
    );
    G4Box* solid_big_box = new G4Box(
        "big_box",
        world_radius,
        world_radius,
        world_radius
    );
    G4LogicalVolume* logical_big_box;
    //scintillator_centre_distance / 2.0 + 2.0 * scintillator_half_size[2]
    G4LogicalVolume* logical_world = new G4LogicalVolume(
        solid_world,
        world_material,
        world_name
    );
    G4VPhysicalVolume* physical_world = new G4PVPlacement(
        0,
        G4ThreeVector(),
        logical_world,
        world_name,
        0,
        false,
        0,
        check_overlaps
    );

    // Scintillator 1
    //
    G4Box* solid_scintillator_1 = new G4Box(
        scintillator_1_name,
        scintillator_half_size[0],
        scintillator_half_size[1],
        scintillator_half_size[2]
    );
    G4LogicalVolume* logical_scintillator_1 = new G4LogicalVolume(
        solid_scintillator_1,
        scintillator_material,
        scintillator_1_name
    );
    G4VPhysicalVolume* physical_scintillator_1 = new G4PVPlacement(
        0,
        scintillator_1_position,
        logical_scintillator_1,
        scintillator_1_name,
        logical_world,
        false,
        0,
        check_overlaps
    );

    // Scintillator 2
    //
    G4Box* solid_scintillator_2 = new G4Box(
        scintillator_2_name,
        scintillator_half_size[0],
        scintillator_half_size[1],
        scintillator_half_size[2]
    );
    G4LogicalVolume* logical_scintillator_2 = new G4LogicalVolume(
        solid_scintillator_2,
        scintillator_material,
        scintillator_2_name
    );
    G4VPhysicalVolume* physical_scintillator_2 = new G4PVPlacement(
        0,
        scintillator_2_position,
        logical_scintillator_2,
        scintillator_2_name,
        logical_world,
        false,
        0,
        check_overlaps
    );

    // Aluminum foil 1
    //
    G4Box* solid_al_foil_1_and_scintillator = new G4Box(
        al_foil_1_name + scintillator_1_name,
        scintillator_half_size[0] + al_foil_thickness,
        scintillator_half_size[1] + al_foil_thickness,
        scintillator_half_size[2] + al_foil_thickness
    );
    G4SubtractionSolid* solid_al_foil_1 = new G4SubtractionSolid(
        al_foil_1_name,
        solid_al_foil_1_and_scintillator,
        solid_scintillator_1
    );
    G4LogicalVolume* logic_al_foil_1 = new G4LogicalVolume(
        solid_al_foil_1,
        al_foil_material,
        al_foil_1_name
    );
    G4VPhysicalVolume* physical_al_foil_1 = new G4PVPlacement(
        0,
        scintillator_1_position,
        logic_al_foil_1,
        al_foil_1_name,
        logical_world,
        false,
        0,
        check_overlaps
    );

    // Aluminum foil 2
    //
    G4Box* solid_al_foil_2_and_scintillator = new G4Box(
        al_foil_2_name + scintillator_2_name,
        scintillator_half_size[0] + al_foil_thickness,
        scintillator_half_size[1] + al_foil_thickness,
        scintillator_half_size[2] + al_foil_thickness
    );
    G4SubtractionSolid* solid_al_foil_2 = new G4SubtractionSolid(
        al_foil_2_name,
        solid_al_foil_2_and_scintillator,
        solid_scintillator_2
    );
    G4LogicalVolume* logic_al_foil_2 = new G4LogicalVolume(
        solid_al_foil_2,
        al_foil_material,
        al_foil_2_name
    );
    G4VPhysicalVolume* physical_al_foil_2 = new G4PVPlacement(
        0,
        scintillator_2_position,
        logic_al_foil_2,
        al_foil_2_name,
        logical_world,
        false,
        0,
        check_overlaps
    );

    return physical_world;
}

