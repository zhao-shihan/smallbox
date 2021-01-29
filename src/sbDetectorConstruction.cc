#include "sbDetectorConstruction.hh"

sbDetectorConstruction* sbDetectorConstruction::sbDCInstance = nullptr;

sbDetectorConstruction::sbDetectorConstruction() :
    fLogicalScintillator(nullptr) {}

G4VPhysicalVolume* sbDetectorConstruction::Construct() {
    // Set if check overlaps
    //
    constexpr G4bool enableCheckOverlaps = true;

    G4NistManager* nist = G4NistManager::Instance();

    // world material
    //
    G4Material* worldMaterial = nist->FindOrBuildMaterial(gWorldMaterialName);

#if SB_ENABLE_OPTICAL_PHYSICS
    SetWorldOpticalProperties(worldMaterial);
#endif

    // world construction

    G4Sphere* solidBigSphere = new G4Sphere(
        "big_sphere",
        0.0,
        gWorldRadius,
        0.0,
        360 * deg,
        0.0,
        360 * deg
    );
    G4Box* solidBigBox = new G4Box(
        "big_box",
        gWorldRadius,
        gWorldRadius,
        gWorldRadius
    );
    const G4ThreeVector bigSphereTransition(0.0, 0.0,
        -gWorldRadius - (gScintillatorDistance / 2.0 + 2.0 * gScintillatorHalfSize[2]) - 5.0 * cm);
    G4SubtractionSolid* solidWorld = new G4SubtractionSolid(
        gWorldName,
        solidBigSphere,
        solidBigBox,
        nullptr,
        bigSphereTransition
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
        enableCheckOverlaps
    );

    // scintillator material
    //
    G4Material* scintillatorMaterial = new G4Material(gScintillatorMaterialName, 1.032 * g / cm3, 2, kStateSolid);
    G4Element* elementH = new G4Element("Hydrogen", "H", 1.0, 1.008 * g / mole);
    G4Element* elementC = new G4Element("Carbon", "C", 6.0, 12.01 * g / mole);
    scintillatorMaterial->AddElement(elementC, 9);
    scintillatorMaterial->AddElement(elementH, 10);

#if SB_ENABLE_OPTICAL_PHYSICS
    SetScintillatorOpticalProperties(scintillatorMaterial);
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

    const G4double scintillator1zPosition = gScintillatorDistance / 2.0 + gScintillatorHalfSize[2];
    const G4ThreeVector scintillator1Position(0.0 * cm, 0.0 * cm, scintillator1zPosition);
    const G4ThreeVector scintillator2Position(0.0 * cm, 0.0 * cm, -scintillator1zPosition);

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
        enableCheckOverlaps
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
        enableCheckOverlaps
    );

    // aluminum foil material
    //
    G4Material* alFoilMaterial = nist->FindOrBuildMaterial(gAlFoilMaterialName);

#if SB_ENABLE_OPTICAL_PHYSICS
    // aluminum foil optical surface
    //
    G4OpticalSurface* alFoilOpticalSurface = new G4OpticalSurface(
        "alFoilOpticalSurface",
        unified,
        polished,
        dielectric_metal
    );
    SetAlFoilOpticalProperties(alFoilOpticalSurface);
#endif

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
        enableCheckOverlaps
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
        enableCheckOverlaps
    );

#if SB_ENABLE_OPTICAL_PHYSICS
    // aluminum foil 1&2 surface construction

    G4LogicalSkinSurface* logicalAlFoil1Surface = new G4LogicalSkinSurface(
        gAlFoilsName.first + "_surface",
        logicalAlFoil1,
        alFoilOpticalSurface
    );
    G4LogicalSkinSurface* logicalAlFoil2Surface = new G4LogicalSkinSurface(
        gAlFoilsName.second + "_surface",
        logicalAlFoil2,
        alFoilOpticalSurface
    );
#endif

    return physicalWorld;
}

void sbDetectorConstruction::ConstructSDandField() {
    sbScintillatorSD* scintillatorSD = new sbScintillatorSD(gSensitiveDetectorName);
    G4SDManager::GetSDMpointer()->AddNewDetector(scintillatorSD);
    SetSensitiveDetector(fLogicalScintillator, scintillatorSD);
}

#if SB_ENABLE_OPTICAL_PHYSICS

void sbDetectorConstruction::SetWorldOpticalProperties(G4Material* worldMaterial) const {
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

void sbDetectorConstruction::SetScintillatorOpticalProperties(G4Material* scintillatorMaterial) const {
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

void sbDetectorConstruction::SetAlFoilOpticalProperties(G4OpticalSurface* alFoilOpticalSurface) const {
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

#endif

