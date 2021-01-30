#include "sbDetectorConstruction.hh"

sbDetectorConstruction* sbDetectorConstruction::sbDCInstance = nullptr;

sbDetectorConstruction::sbDetectorConstruction() :
    fLogicalScintillator(nullptr),
    fLogicalSiPM(nullptr) {}

G4VPhysicalVolume* sbDetectorConstruction::Construct() {
    // Set if check overlaps
    //
    constexpr G4bool checkOverlaps = true;

    G4NistManager* nist = G4NistManager::Instance();

    // ============================================================================
    // world
    // ============================================================================

    // world material
    //
    G4Material* worldMaterial = nist->FindOrBuildMaterial(gWorldMaterialName);
#if SB_ENABLE_OPTICAL_PHYSICS
    SetWorldMaterialProperties(worldMaterial);
#endif

    // world construction

    G4Sphere* solidSphere = new G4Sphere(
        "sphere",
        0.0,
        gWorldRadius,
        0.0,
        360 * deg,
        0.0,
        360 * deg
    );
    G4Box* solidSphereSubtrahend = new G4Box(
        "sphere_subtrahend",
        gWorldRadius,
        gWorldRadius,
        gWorldRadius
    );
    const G4ThreeVector subtrahendTransition(0.0, 0.0,
        -gWorldRadius
        - 0.5 * gScintillatorDistance
        - 2.0 * gScintillatorHalfSize[2]
        - gDistanceBetweenScintillatorAndWorldBottom
    );
    G4SubtractionSolid* solidWorld = new G4SubtractionSolid(
        gWorldName,
        solidSphere,
        solidSphereSubtrahend,
        nullptr,
        subtrahendTransition
    );
    G4LogicalVolume* logicalWorld = new G4LogicalVolume(
        solidWorld,
        worldMaterial,
        gWorldName
    );
    G4VPhysicalVolume* physicalWorld = new G4PVPlacement(
        0,
        G4ThreeVector(),
        logicalWorld,
        gWorldName,
        0,
        false,
        0,
        checkOverlaps
    );

    // ============================================================================
    // scintillators
    // ============================================================================

    // scintillator material
    //
    G4Material* scintillatorMaterial = new G4Material(gScintillatorMaterialName, 1.032 * g / cm3, 2, kStateSolid);
    G4Element* elementH = new G4Element("Hydrogen", "H", 1.0, 1.008 * g / mole);
    G4Element* elementC = new G4Element("Carbon", "C", 6.0, 12.01 * g / mole);
    scintillatorMaterial->AddElement(elementC, 9);
    scintillatorMaterial->AddElement(elementH, 10);
#if SB_ENABLE_OPTICAL_PHYSICS
    SetScintillatorMaterialProperties(scintillatorMaterial);
#endif

    // solid & logical scintillator construction

    G4Box* solidScintillator = new G4Box(
        "scintillator",
        gScintillatorHalfSize[0],
        gScintillatorHalfSize[1],
        gScintillatorHalfSize[2]
    );
    this->fLogicalScintillator = new G4LogicalVolume(
        solidScintillator,
        scintillatorMaterial,
        "scintillator"
    );

    // scintillators' position

    const G4double scintillator1zPosition = 0.5 * gScintillatorDistance + gScintillatorHalfSize[2];
    const G4ThreeVector scintillator1Position(0.0, 0.0, scintillator1zPosition);
    const G4ThreeVector scintillator2Position(0.0, 0.0, -scintillator1zPosition);

    // physical scintillator 1 construction

    // G4VPhysicalVolume* physicalScintillator1 =
    new G4PVPlacement(
        0,
        scintillator1Position,
        fLogicalScintillator,
        gScintillatorsName.first,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // physical scintillator 2 construction

    // G4VPhysicalVolume* physicalScintillator2 =
    new G4PVPlacement(
        0,
        scintillator2Position,
        fLogicalScintillator,
        gScintillatorsName.second,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // ============================================================================
    // aluminum foils
    // ============================================================================

    // aluminum foil material
    //
    G4Material* alFoilMaterial = nist->FindOrBuildMaterial(gAlFoilMaterialName);

    // solid & logical aluminum foil construction

    G4Box* solidAlFoilAndScintillator = new G4Box(
        "al_foil_and_scintillator",
        gScintillatorHalfSize[0] + gAlFoilScintillatorGap + gAlFoilThickness,
        gScintillatorHalfSize[1] + gAlFoilScintillatorGap + gAlFoilThickness,
        gScintillatorHalfSize[2] + gAlFoilScintillatorGap + gAlFoilThickness
    );
    G4Box* solidVolumeInsideAlFoil = new G4Box(
        "al_foil_subtrahend",
        gScintillatorHalfSize[0] + gAlFoilScintillatorGap,
        gScintillatorHalfSize[1] + gAlFoilScintillatorGap,
        gScintillatorHalfSize[2] + gAlFoilScintillatorGap
    );
    G4SubtractionSolid* solidAlFoilWithoutHole = new G4SubtractionSolid(
        "al_foil_without_hole",
        solidAlFoilAndScintillator,
        solidVolumeInsideAlFoil
    );
    G4Box* solidHole = new G4Box(
        "al_foil_hole",
        gAlFoilHoleHalfWidth,
        gAlFoilHoleHalfWidth,
        0.5 * gAlFoilThickness
    );

    // aluminum foil 1 construction

    G4SubtractionSolid* solidAlFoil1 = new G4SubtractionSolid(
        gAlFoilsName.first,
        solidAlFoilWithoutHole,
        solidHole,
        nullptr,
        G4ThreeVector(0, 0, gScintillatorHalfSize[2] + gAlFoilScintillatorGap + 0.5 * gAlFoilThickness)
    );
    G4LogicalVolume* logicalAlFoil1 = new G4LogicalVolume(
        solidAlFoil1,
        alFoilMaterial,
        gAlFoilsName.first
    );
    // G4VPhysicalVolume* physicalAlFoil1 = 
    new G4PVPlacement(
        0,
        scintillator1Position,
        logicalAlFoil1,
        gAlFoilsName.first,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // aluminum foil 2 construction

    G4SubtractionSolid* solidAlFoil2 = new G4SubtractionSolid(
        gAlFoilsName.second,
        solidAlFoilWithoutHole,
        solidHole,
        nullptr,
        G4ThreeVector(0, 0, -gScintillatorHalfSize[2] - gAlFoilScintillatorGap - 0.5 * gAlFoilThickness)
    );
    G4LogicalVolume* logicalAlFoil2 = new G4LogicalVolume(
        solidAlFoil2,
        alFoilMaterial,
        gAlFoilsName.second
    );
    // G4VPhysicalVolume* physicalAlFoil2 = 
    new G4PVPlacement(
        0,
        scintillator2Position,
        logicalAlFoil2,
        gAlFoilsName.second,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

#if SB_ENABLE_OPTICAL_PHYSICS
    // aluminum foil 1&2 optical surface construction

    // aluminum foil optical surface
    //
    G4OpticalSurface* alFoilOpticalSurface = new G4OpticalSurface(
        "alFoilOpticalSurface",
        unified,
        polished,
        dielectric_metal
    );
    SetAlFoilSurfaceProperties(alFoilOpticalSurface);
    // G4LogicalSkinSurface* logicalAlFoil1Surface = 
    new G4LogicalSkinSurface(
        gAlFoilsName.first + "_surface",
        logicalAlFoil1,
        alFoilOpticalSurface
    );
    // G4LogicalSkinSurface* logicalAlFoil2Surface = 
    new G4LogicalSkinSurface(
        gAlFoilsName.second + "_surface",
        logicalAlFoil2,
        alFoilOpticalSurface
    );
#endif

    // ============================================================================
    // SiPMs
    // ============================================================================

    // SiPM material
    //
    G4Material* SiPMMaterial = nist->FindOrBuildMaterial(gSiPMMaterialName);

    // solid & logical SiPM construction

    G4Box* solidSiPM = new G4Box(
        "SiPM",
        gSiPMHalfSize[0],
        gSiPMHalfSize[1],
        gSiPMHalfSize[2]
    );
    this->fLogicalSiPM = new G4LogicalVolume(
        solidSiPM,
        SiPMMaterial,
        "SiPM"
    );

    // SiPMs' position

    const G4double SiPM1zPosition =
        0.5 * gScintillatorDistance + 2.0 * gScintillatorHalfSize[2] + gSiPMHalfSize[2];
    const G4ThreeVector SiPM1Position(0.0, 0.0, SiPM1zPosition);
    const G4ThreeVector SiPM2Position(0.0, 0.0, -SiPM1zPosition);

    // physical SiPM 1 construction

    // G4VPhysicalVolume* physicalSiPM1 =
    new G4PVPlacement(
        0,
        SiPM1Position,
        fLogicalSiPM,
        gSiPMsName.first,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // physical SiPM 2 construction

    // G4VPhysicalVolume* physicalSiPM2 =
    new G4PVPlacement(
        0,
        SiPM2Position,
        fLogicalSiPM,
        gSiPMsName.second,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

#if SB_ENABLE_OPTICAL_PHYSICS
    // SiPM 1&2 surface construction
    
    // SiPM optical surface
    //
    G4OpticalSurface* SiPMOpticalSurface = new G4OpticalSurface(
        "alFoilOpticalSurface",
        unified,
        polished,
        dielectric_metal
    );
    SetSiPMSurfaceProperties(SiPMOpticalSurface);
    // G4LogicalSkinSurface* logicalSiPM1Surface = 
    new G4LogicalSkinSurface(
        gSiPMsName.first + "_surface",
        fLogicalSiPM,
        SiPMOpticalSurface
    );
    // G4LogicalSkinSurface* logicalSiPM2Surface = 
    new G4LogicalSkinSurface(
        gSiPMsName.second + "_surface",
        fLogicalSiPM,
        SiPMOpticalSurface
    );
#endif

    // ============================================================================
    // PCBs
    // ============================================================================

    // PCB material
    //
    G4Material* PCBMaterial = nist->FindOrBuildMaterial(gPCBMaterialName);

    // solid & logical PCB construction

    G4Box* solidPCB = new G4Box(
        "PCB",
        gPCBHalfSize[0],
        gPCBHalfSize[1],
        gPCBHalfSize[2]
    );
    G4LogicalVolume* logicalPCB = new G4LogicalVolume(
        solidPCB,
        PCBMaterial,
        "PCB"
    );

    // PCBs' position

    const G4double PCB1zPosition =
        0.5 * gScintillatorDistance + 2.0 * gScintillatorHalfSize[2] + 2.0 * gSiPMHalfSize[2] + gPCBHalfSize[2];
    const G4ThreeVector PCB1Position(0.0, 0.0, PCB1zPosition);
    const G4ThreeVector PCB2Position(0.0, 0.0, -PCB1zPosition);

    // physical PCB 1 construction

    // G4VPhysicalVolume* physicalPCB1 =
    new G4PVPlacement(
        0,
        PCB1Position,
        logicalPCB,
        gPCBsName.first,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // physical PCB 2 construction

    // G4VPhysicalVolume* physicalPCB2 =
    new G4PVPlacement(
        0,
        PCB2Position,
        logicalPCB,
        gPCBsName.second,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    return physicalWorld;
}

void sbDetectorConstruction::ConstructSDandField() {
    auto SDManager = G4SDManager::GetSDMpointer();

    sbScintillatorSD* scintillatorSD = new sbScintillatorSD(gScintillatorSDName);
    SDManager->AddNewDetector(scintillatorSD);
    SetSensitiveDetector(fLogicalScintillator, scintillatorSD);

    sbSiPMSD* SiPMSD = new sbSiPMSD(gSiPMSDName);
    SDManager->AddNewDetector(SiPMSD);
    SetSensitiveDetector(fLogicalSiPM, SiPMSD);
}

#if SB_ENABLE_OPTICAL_PHYSICS

void sbDetectorConstruction::SetWorldMaterialProperties(G4Material* worldMaterial) const {
    G4MaterialPropertiesTable* worldPropertiesTable = new G4MaterialPropertiesTable();

    // Refraction index
    G4double refractionPhotonEnergy[2] = { 1.0 * eV, 20.0 * eV };
    G4double refractionIndex[2] = { 1.0, 1.0 };
    worldPropertiesTable->AddProperty(
        "RINDEX",
        refractionPhotonEnergy,
        refractionIndex,
        2
    );

    // Set!
    worldMaterial->SetMaterialPropertiesTable(worldPropertiesTable);
}

void sbDetectorConstruction::SetScintillatorMaterialProperties(G4Material* scintillatorMaterial) const {
    G4MaterialPropertiesTable* scintillatorPropertiesTable = new G4MaterialPropertiesTable();
    auto scintillatorProperties(CreateMapFromCSV<G4double>("./datafiles/scintillatorProperties.csv"));

    scintillatorPropertiesTable->AddProperty(
        "RINDEX",
        &scintillatorProperties["RINDEX_energy"][0],
        &scintillatorProperties["RINDEX"][0],
        scintillatorProperties["RINDEX"].size()
    );
#if !SB_SUPPRESS_SCINTILLATION_PROPAGATION
    scintillatorPropertiesTable->AddProperty(
        "ABSLENGTH",
        &scintillatorProperties["ABSLENGTH_energy"][0],
        &scintillatorProperties["ABSLENGTH"][0],
        scintillatorProperties["ABSLENGTH"].size()
    );
#else
    // Surppress light propagation
    G4double absorptionLengthPhotonEnergy[2] = { 1.0 * eV, 20.0 * eV };
    G4double absorptionLength[2] = { 0.0, 0.0 };
    scintillatorPropertiesTable->AddProperty(
        "ABSLENGTH",
        absorptionLengthPhotonEnergy,
        absorptionLength,
        2
    );
#endif
    scintillatorPropertiesTable->AddProperty(
        "RAYLEIGH",
        &scintillatorProperties["RAYLEIGH_energy"][0],
        &scintillatorProperties["RAYLEIGH"][0],
        scintillatorProperties["RAYLEIGH"].size()
    );
    scintillatorPropertiesTable->AddProperty(
        "FASTCOMPONENT",
        &scintillatorProperties["FASTCOMPONENT_energy"][0],
        &scintillatorProperties["FASTCOMPONENT"][0],
        scintillatorProperties["FASTCOMPONENT"].size()
    );
    scintillatorPropertiesTable->AddProperty(
        "SLOWCOMPONENT",
        &scintillatorProperties["SLOWCOMPONENT_energy"][0],
        &scintillatorProperties["SLOWCOMPONENT"][0],
        scintillatorProperties["SLOWCOMPONENT"].size()
    );
    scintillatorPropertiesTable->AddProperty(
        "REEMISSIONPROB",
        &scintillatorProperties["REEMISSIONPROB_energy"][0],
        &scintillatorProperties["REEMISSIONPROB"][0],
        scintillatorProperties["REEMISSIONPROB"].size()
    );
    scintillatorPropertiesTable->
        AddConstProperty("SCINTILLATIONYIELD", scintillatorProperties["SCINTILLATIONYIELD"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("RESOLUTIONSCALE", scintillatorProperties["RESOLUTIONSCALE"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("FASTTIMECONSTANT", scintillatorProperties["FASTTIMECONSTANT"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("SLOWTIMECONSTANT", scintillatorProperties["SLOWTIMECONSTANT"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("YIELDRATIO", scintillatorProperties["YIELDRATIO"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("AlphaFASTTIMECONSTANT", scintillatorProperties["AlphaFASTTIMECONSTANT"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("AlphaSLOWTIMECONSTANT", scintillatorProperties["AlphaSLOWTIMECONSTANT"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("AlphaYIELDRATIO", scintillatorProperties["AlphaYIELDRATIO"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("NeutronFASTTIMECONSTANT", scintillatorProperties["NeutronFASTTIMECONSTANT"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("NeutronSLOWTIMECONSTANT", scintillatorProperties["NeutronSLOWTIMECONSTANT"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("NeutronYIELDRATIO", scintillatorProperties["NeutronYIELDRATIO"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("ReemissionFASTTIMECONSTANT", scintillatorProperties["ReemissionFASTTIMECONSTANT"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("ReemissionSLOWTIMECONSTANT", scintillatorProperties["ReemissionSLOWTIMECONSTANT"][0]);
    scintillatorPropertiesTable->
        AddConstProperty("ReemissionYIELDRATIO", scintillatorProperties["ReemissionYIELDRATIO"][0]);

    scintillatorMaterial->SetMaterialPropertiesTable(scintillatorPropertiesTable);

    scintillatorMaterial->GetIonisation()->SetBirksConstant(0.15 * mm / MeV);
}

void sbDetectorConstruction::SetAlFoilSurfaceProperties(G4OpticalSurface* alFoilOpticalSurface) const {
    G4MaterialPropertiesTable* alFoilPropertiesTable = new G4MaterialPropertiesTable();

    // Reflectivity
    G4double reflectionPhotonEnergy[2] = { 1.0 * eV, 20.0 * eV };
#if SB_ENABLE_AL_FOIL_REFLECTION
    G4double reflectivity[2] = { 0.9, 0.9 };
#else
    G4double reflectivity[2] = { 0.0, 0.0 };
#endif
    alFoilPropertiesTable->AddProperty(
        "REFLECTIVITY",
        reflectionPhotonEnergy,
        reflectivity,
        2
    );

    // Set!
    alFoilOpticalSurface->SetMaterialPropertiesTable(alFoilPropertiesTable);
}

void sbDetectorConstruction::SetSiPMSurfaceProperties(G4OpticalSurface* SiPMOpticalSurface) const {
    G4MaterialPropertiesTable* SiPMPropertiesTable = new G4MaterialPropertiesTable();

    // Reflectivity
    G4double reflectionPhotonEnergy[2] = { 1.0 * eV, 20.0 * eV };
    G4double reflectivity[2] = { 0.3, 0.3 };
    SiPMPropertiesTable->AddProperty(
        "REFLECTIVITY",
        reflectionPhotonEnergy,
        reflectivity,
        2
    );

    // Set!
    SiPMOpticalSurface->SetMaterialPropertiesTable(SiPMPropertiesTable);
}

#endif

