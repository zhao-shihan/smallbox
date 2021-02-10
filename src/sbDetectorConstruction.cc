#include "sbDetectorConstruction.hh"

sbDetectorConstruction* sbDetectorConstruction::sbDCInstance = nullptr;

sbDetectorConstruction::sbDetectorConstruction() :
    fLogicalScintillator(nullptr),
    fPhysicalScintillators(nullptr, nullptr),
    fLogicalSiPM(nullptr),
    fPhysicalSiPMs(nullptr, nullptr) {}

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

    G4Tubs* solidWorld = new G4Tubs(
        gWorldName,
        0.0,
        gWorldRadius,
        gWorldHalfHeight,
        0,
        2.0 * M_PI
    );
    G4LogicalVolume* logicalWorld = new G4LogicalVolume(
        solidWorld,
        worldMaterial,
        gWorldName
    );
    G4VPhysicalVolume* physicalWorld = new G4PVPlacement(
        nullptr,
        G4ThreeVector(),
        logicalWorld,
        gWorldName,
        nullptr,
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
    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    scintillatorMaterial->AddElement(C, 9);
    scintillatorMaterial->AddElement(H, 10);
#if SB_ENABLE_OPTICAL_PHYSICS
    SetScintillatorMaterialProperties(scintillatorMaterial);
#endif

    // solid & logical scintillator construction

    G4Box* solidScintillator = new G4Box(
        gScintillatorGeneralName,
        gScintillatorHalfSize[0],
        gScintillatorHalfSize[1],
        gScintillatorHalfSize[2]
    );
    this->fLogicalScintillator = new G4LogicalVolume(
        solidScintillator,
        scintillatorMaterial,
        gScintillatorGeneralName
    );

    // physical upper scintillator construction

    this->fPhysicalScintillators.first = new G4PVPlacement(
        nullptr,
        gScintillatorsPosition.first,
        fLogicalScintillator,
        gScintillatorsName.first,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // physical lower scintillator construction

    this->fPhysicalScintillators.second = new G4PVPlacement(
        nullptr,
        gScintillatorsPosition.second,
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
        gAlFoilGeneralName + "_and_" + gScintillatorGeneralName,
        gScintillatorHalfSize[0] + gAlFoilScintillatorGap + gAlFoilThickness,
        gScintillatorHalfSize[1] + gAlFoilScintillatorGap + gAlFoilThickness,
        gScintillatorHalfSize[2] + gAlFoilScintillatorGap + gAlFoilThickness
    );
    G4Box* solidVolumeInsideAlFoil = new G4Box(
        gAlFoilGeneralName + "_subtrahend",
        gScintillatorHalfSize[0] + gAlFoilScintillatorGap,
        gScintillatorHalfSize[1] + gAlFoilScintillatorGap,
        gScintillatorHalfSize[2] + gAlFoilScintillatorGap
    );
    G4SubtractionSolid* solidAlFoilWithoutHole = new G4SubtractionSolid(
        gAlFoilGeneralName + "_without_hole",
        solidAlFoilAndScintillator,
        solidVolumeInsideAlFoil
    );
    G4Box* solidHole = new G4Box(
        gAlFoilGeneralName + "_hole",
        gAlFoilHoleHalfWidth,
        gAlFoilHoleHalfWidth,
        0.5 * gAlFoilThickness
    );
    G4SubtractionSolid* solidAlFoil = new G4SubtractionSolid(
        gAlFoilGeneralName,
        solidAlFoilWithoutHole,
        solidHole,
        nullptr,
        G4ThreeVector(0, 0, gScintillatorHalfSize[2] + gAlFoilScintillatorGap + 0.5 * gAlFoilThickness)
    );
    G4LogicalVolume* logicalAlFoil = new G4LogicalVolume(
        solidAlFoil,
        alFoilMaterial,
        gAlFoilGeneralName
    );

    // physical upper aluminum foils construction

    new G4PVPlacement(
        nullptr,
        gScintillatorsPosition.first,
        logicalAlFoil,
        gAlFoilsName.first,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // physical lower aluminum foils construction

    auto filp = new G4RotationMatrix(G4ThreeVector(1.0, 0.0, 0.0), M_PI);
    new G4PVPlacement(
        filp,
        gScintillatorsPosition.second,
        logicalAlFoil,
        gAlFoilsName.second,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

#if SB_ENABLE_OPTICAL_PHYSICS
    // aluminum foil optical surface construction

    // aluminum foil optical surface
    //
    G4OpticalSurface* alFoilOpticalSurface = new G4OpticalSurface(
        gAlFoilGeneralName + "_optical_surface",
        unified,
        polished,
        dielectric_metal
    );
    SetAlFoilSurfaceProperties(alFoilOpticalSurface);
    new G4LogicalSkinSurface(
        gAlFoilGeneralName + "_surface",
        logicalAlFoil,
        alFoilOpticalSurface
    );
#endif

    // ============================================================================
    // SiPMs
    // ============================================================================

    // SiPM material
    //
    G4Material* SiPMMaterial = nist->FindOrBuildMaterial(gSiPMMaterialName);
#if SB_ENABLE_OPTICAL_PHYSICS
    SetSiPMMaterialProperties(SiPMMaterial);
#endif

    // solid & logical SiPM construction

    G4Box* solidSiPM = new G4Box(
        gSiPMGeneralName,
        gSiPMHalfSize[0],
        gSiPMHalfSize[1],
        gSiPMHalfSize[2]
    );
    this->fLogicalSiPM = new G4LogicalVolume(
        solidSiPM,
        SiPMMaterial,
        gSiPMGeneralName
    );

    // physical upper SiPM construction

    this->fPhysicalSiPMs.first = new G4PVPlacement(
        nullptr,
        gSiPMsPosition.first,
        fLogicalSiPM,
        gSiPMsName.first,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // physical lower SiPM construction

    this->fPhysicalSiPMs.second = new G4PVPlacement(
        nullptr,
        gSiPMsPosition.second,
        fLogicalSiPM,
        gSiPMsName.second,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // ============================================================================
    // light guides
    // ============================================================================

    // light guide materials
    // 
    G4Material* lightGuideMaterial = new G4Material(gLightGuideMaterialName, 0.97 * g / cm3, 4, kStateLiquid);
    G4Element* O = nist->FindOrBuildElement("O");
    G4Element* Si = nist->FindOrBuildElement("Si");
    lightGuideMaterial->AddElement(C, 2);
    lightGuideMaterial->AddElement(H, 6);
    lightGuideMaterial->AddElement(Si, 1);
    lightGuideMaterial->AddElement(O, 1);
#if SB_ENABLE_OPTICAL_PHYSICS
    SetLightGuideMaterialProperties(lightGuideMaterial);
#endif

    // solid & logical light guide construction

    G4Box* solidLightGuideAndSiPM = new G4Box(
        gLightGuideGeneralName + "_and_" + gSiPMGeneralName,
        gLightGuideHalfWidth,
        gLightGuideHalfWidth,
        0.5 * gLightGuideThickness
    );
    G4SubtractionSolid* solidLightGuide = new G4SubtractionSolid(
        gLightGuideGeneralName,
        solidLightGuideAndSiPM,
        solidSiPM,
        nullptr,
        G4ThreeVector(0, 0, gSiPMScintillatorGap - (0.5 * gLightGuideThickness - gSiPMHalfSize[2]))
    );
    G4LogicalVolume* logicalLightGuide = new G4LogicalVolume(
        solidLightGuide,
        lightGuideMaterial,
        gLightGuideGeneralName
    );

    // physical upper light guide construction

    new G4PVPlacement(
        nullptr,
        gLightGuidesPosition.first,
        logicalLightGuide,
        gLightGuidesName.first,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // physical lower light guide construction

    new G4PVPlacement(
        filp,
        gLightGuidesPosition.second,
        logicalLightGuide,
        gLightGuidesName.second,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // ============================================================================
    // PCBs
    // ============================================================================

    // PCB material
    //
    G4Material* PCBMaterial = nist->FindOrBuildMaterial(gPCBMaterialName);

    // solid & logical PCB construction

    G4Box* solidPCB = new G4Box(
        gPCBGeneralName,
        gPCBHalfSize[0],
        gPCBHalfSize[1],
        gPCBHalfSize[2]
    );
    G4LogicalVolume* logicalPCB = new G4LogicalVolume(
        solidPCB,
        PCBMaterial,
        gPCBGeneralName
    );

    // physical PCB 1 construction

    new G4PVPlacement(
        nullptr,
        gPCBsPosition.first,
        logicalPCB,
        gPCBsName.first,
        logicalWorld,
        false,
        0,
        checkOverlaps
    );

    // physical PCB 2 construction

    new G4PVPlacement(
        nullptr,
        gPCBsPosition.second,
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
#if SB_PROCESS_SCINTILLATOR_HIT || SB_PROCESS_SIPM_HIT
    auto SDManager = G4SDManager::GetSDMpointer();
#endif
#if SB_PROCESS_SCINTILLATOR_HIT
    sbScintillatorSD* scintillatorSD = new sbScintillatorSD(gScintillatorSDName);
    SDManager->AddNewDetector(scintillatorSD);
    SetSensitiveDetector(fLogicalScintillator, scintillatorSD);
#endif
#if SB_PROCESS_SIPM_HIT
    sbSiPMSD* SiPMSD = new sbSiPMSD(gSiPMSDName);
    SDManager->AddNewDetector(SiPMSD);
    SetSensitiveDetector(fLogicalSiPM, SiPMSD);
#endif
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
#if SB_KILL_SCINTILLATION_PHOTON
    // Kill scintillation photon.
    G4double absorptionLengthPhotonEnergy[2] = { 1.0 * eV, 20.0 * eV };
    G4double absorptionLength[2] = { 0.0, 0.0 };
    scintillatorPropertiesTable->AddProperty(
        "ABSLENGTH",
        absorptionLengthPhotonEnergy,
        absorptionLength,
        2
    );
#else
    scintillatorPropertiesTable->AddProperty(
        "ABSLENGTH",
        &scintillatorProperties["ABSLENGTH_energy"][0],
        &scintillatorProperties["ABSLENGTH"][0],
        scintillatorProperties["ABSLENGTH"].size()
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

void sbDetectorConstruction::SetSiPMMaterialProperties(G4Material* SiPMMaterial) const {
    G4MaterialPropertiesTable* SiPMPropertiesTable = new G4MaterialPropertiesTable();

    // Refraction index
    G4double refractionPhotonEnergy[2] = { 1.587 * eV, 3.095 * eV };
    G4double refractionIndex[2] = { 1.403, 1.406 };
    SiPMPropertiesTable->AddProperty(
        "RINDEX",
        refractionPhotonEnergy,
        refractionIndex,
        2
    );
    // Absorption length
    G4double absorptionLengthPhotonEnergy[2] = { 1.0 * eV, 20.0 * eV };
    G4double absorptionLength[2] = { 0.0, 0.0 };
    SiPMPropertiesTable->AddProperty(
        "ABSLENGTH",
        absorptionLengthPhotonEnergy,
        absorptionLength,
        2
    );

    // Set!
    SiPMMaterial->SetMaterialPropertiesTable(SiPMPropertiesTable);
}

void sbDetectorConstruction::SetLightGuideMaterialProperties(G4Material* lightGuideMaterial) const {
    G4MaterialPropertiesTable* lightGuidePropertiesTable = new G4MaterialPropertiesTable();

    // Refraction index
    G4double refractionPhotonEnergy[2] = { 1.587 * eV, 3.095 * eV };
    G4double refractionIndex[2] = { 1.403, 1.406 };
    lightGuidePropertiesTable->AddProperty(
        "RINDEX",
        refractionPhotonEnergy,
        refractionIndex,
        2
    );

    // Set!
    lightGuideMaterial->SetMaterialPropertiesTable(lightGuidePropertiesTable);
}

#endif

