#include "SBDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4UnionSolid.hh"
#include "G4QuadrangularFacet.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4AssemblyVolume.hh"

SBDetectorConstruction::SBDetectorConstruction() :
    fWorldName("world"),

    fScintillatorHalfSize{ 5 * cm, 5 * cm, 0.5 * cm },
    fScintillatorName("scintillator"),

    fAcrylicLightGuideSiPMSideHalfSize{ 1 * cm, 1 * cm },
    fAcrylicLightGuideLength(2 * cm),
    fAcrylicName("arcylic"),

    fAcrylicScintillatorGap(50 * um),
    fAcrylicScintillatorGapName("arcylic_scintillator_gap"),

    fCoverScintillatorGap(50 * um),
    fCoverScintillatorGapName("cover_scintillator_gap"),

    fCoverThickness(100 * um),
    fCoverHoleHalfWidth(4 * mm),
    fCoverName("cover"),

    fSiPMCount(1 * 2),
    fSiPMHalfSize{ 3.5 * mm, 3.5 * mm, 0.25 * mm },
    fSiPMScintillatorGap(30 * um),
    fSiPMName("SiPM"),

    fSiliconeOilHalfWidth(fCoverHoleHalfWidth),
    fSiliconeOilThickness(fCoverScintillatorGap + fCoverThickness),
    fSiliconeOilName("silicone_oil"),

    fPCBHalfSize{ 1.25 * cm, 1.25 * cm, 0.5 * mm },
    fPCBName("PCB"),

    fDetectorDistance(1.1 * cm),

    fTempName("_tmp"),

    fLogicalScintillator(nullptr),
    fLogicalSiPM(nullptr),

    fDetectorPartCount(0) {}

SBDetectorConstruction::~SBDetectorConstruction() {}

G4VPhysicalVolume* SBDetectorConstruction::Construct() {
    //////////////////////////////////////////////////////////////////////////////////
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //
    //                              Step 1: material                                //
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< //
    //////////////////////////////////////////////////////////////////////////////////

    // NIST manager.
    G4NistManager* nist = G4NistManager::Instance();

    // Construct materials.
    //
    G4PVDataVector energies, values; // Material properties.
    auto H = nist->FindOrBuildElement("H");
    auto C = nist->FindOrBuildElement("C");
    auto O = nist->FindOrBuildElement("O");
    auto Si = nist->FindOrBuildElement("Si");

    // ============================================================================
    // world material
    // ============================================================================

    // worldRefractionIndex
    energies = { 1.0 * eV, 20.0 * eV };
    values = { 1.0, 1.0 };
    auto worldRefractionIndex = new G4MaterialPropertyVector(&energies.front(), &values.front(), energies.size());

    // world material: void
    //
    auto worldMaterial = nist->BuildMaterialWithNewDensity("void", "G4_AIR", 1e-24 * g / cm3);
    auto worldPropertiesTable = new G4MaterialPropertiesTable();
    worldPropertiesTable->AddProperty("RINDEX", worldRefractionIndex);
    worldMaterial->SetMaterialPropertiesTable(worldPropertiesTable);

    // ============================================================================
    // scintillator material
    // ============================================================================

    // scintillator material: plastic scintillator
    //
    auto scintillatorMaterial = new G4Material("plastic_scintillator", 1.032 * g / cm3, 2, kStateSolid);
    scintillatorMaterial->AddElement(C, 9);
    scintillatorMaterial->AddElement(H, 10);
    SetScintillatorMaterialProperties(scintillatorMaterial);

    // ============================================================================
    // acrylic material
    // ============================================================================

    // acrylic RefractionIndex
    energies = { 1.587 * eV, 3.095 * eV };
    values = { 1.490, 1.492 };
    auto acrylicRefractionIndex = new G4MaterialPropertyVector(&energies.front(), &values.front(), energies.size());

    // acrylic
    //
    auto acrylicMaterial = new G4Material("acrylic", 1.16 * g / cm3, 3, kStateSolid);
    acrylicMaterial->AddElement(C, 5);
    acrylicMaterial->AddElement(H, 8);
    acrylicMaterial->AddElement(O, 2);
    auto acrylicMaterialPropertiesTable = new G4MaterialPropertiesTable();
    acrylicMaterialPropertiesTable->AddProperty("RINDEX", acrylicRefractionIndex);
    acrylicMaterial->SetMaterialPropertiesTable(acrylicMaterialPropertiesTable);

    // ============================================================================
    // silicone oil material
    // ============================================================================

    // silicone oil RefractionIndex
    energies = { 1.587 * eV, 3.095 * eV };
    values = { 1.403, 1.406 };
    auto siliconeOilRefractionIndex = new G4MaterialPropertyVector(&energies.front(), &values.front(), energies.size());

    // silicone oil absoption length
    energies = { 1.587 * eV, 3.095 * eV };
    values = { 15 * cm, 15 * cm };
    auto siliconeOilAbsLength = new G4MaterialPropertyVector(&energies.front(), &values.front(), energies.size());

    // silicone oil
    // 
    auto siliconeOilMaterial = new G4Material("silicone_oil", 0.97 * g / cm3, 4, kStateLiquid);
    siliconeOilMaterial->AddElement(C, 2);
    siliconeOilMaterial->AddElement(H, 6);
    siliconeOilMaterial->AddElement(Si, 1);
    siliconeOilMaterial->AddElement(O, 1);
    auto siliconeOilPropertiesTable = new G4MaterialPropertiesTable();
    siliconeOilPropertiesTable->AddProperty("RINDEX", siliconeOilRefractionIndex);
    siliconeOilPropertiesTable->AddProperty("ABSLENGTH", siliconeOilAbsLength);
    siliconeOilMaterial->SetMaterialPropertiesTable(siliconeOilPropertiesTable);

    // ============================================================================
    // gap between cover and scintillator material
    // ============================================================================

    // gapRefractionIndex
    energies = { 1.587 * eV, 3.095 * eV };
    values = { 1.36, 1.36 };
    auto gapRefractionIndex = new G4MaterialPropertyVector(&energies.front(), &values.front(), energies.size());

    // gap absoption length
    energies = { 1.587 * eV, 3.095 * eV };
    values = { 15 * cm, 15 * cm };
    auto gapAbsLength = new G4MaterialPropertyVector(&energies.front(), &values.front(), energies.size());

    // gap material: glue
    //
    auto gapMaterial = nist->FindOrBuildMaterial("G4_WATER");
    auto gapPropertiesTable = new G4MaterialPropertiesTable();
    gapPropertiesTable->AddProperty("RINDEX", gapRefractionIndex);
    gapPropertiesTable->AddProperty("ABSLENGTH", gapAbsLength);
    gapMaterial->SetMaterialPropertiesTable(gapPropertiesTable);

    // ============================================================================
    // cover material
    // ============================================================================

    // coverReflectivity
    energies = { 1.0 * eV, 20.0 * eV };
    values = { 0.95, 0.95 };
    auto coverReflectivity = new G4MaterialPropertyVector(&energies.front(), &values.front(), energies.size());

    // cover material: Al
    //
    auto coverMaterial = nist->FindOrBuildMaterial("G4_Al");
    auto coverPropertiesTable = new G4MaterialPropertiesTable();
    coverPropertiesTable->AddProperty("REFLECTIVITY", coverReflectivity);
    // coverPropertiesTable set for optical surface.

    // ============================================================================
    // SiPM material
    // ============================================================================

    // SiPM material: Si
    //
    auto SiPMMaterial = nist->FindOrBuildMaterial("G4_Si");
    auto SiPMPropertiesTable = new G4MaterialPropertiesTable();
    SiPMPropertiesTable->AddProperty("RINDEX", siliconeOilRefractionIndex);
    G4double absorptionLengthPhotonEnergy[2] = { 1.0 * eV, 20.0 * eV };
    G4double absorptionLength[2] = { 0.0, 0.0 };
    SiPMPropertiesTable->AddProperty("ABSLENGTH", absorptionLengthPhotonEnergy, absorptionLength, 2);
    SiPMMaterial->SetMaterialPropertiesTable(SiPMPropertiesTable);

    // ============================================================================
    // PCB material
    // ============================================================================

    // PCB material: polycarbonate
    //
    auto PCBMaterial = nist->FindOrBuildMaterial("G4_POLYCARBONATE");

    //////////////////////////////////////////////////////////////////////////////////
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //
    //                              Step 2: Geometry                                //
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< //
    //////////////////////////////////////////////////////////////////////////////////

    // Set if check overlaps
    //
    constexpr G4bool checkOverlaps = true;

    // ============================================================================
    // world geometry
    // ============================================================================

    // world construction

    fWorldZMax = 0.5 * fDetectorDistance + fScintillatorHalfSize[2] + fAcrylicLightGuideLength + 1 * cm;
    auto solidWorld = new G4Box(
        fWorldName,
        fScintillatorHalfSize[0] + 1 * cm,
        fScintillatorHalfSize[1] + 1 * cm,
        fWorldZMax
    );
    auto logicalWorld = new G4LogicalVolume(
        solidWorld,
        worldMaterial,
        fWorldName
    );
    auto physicalWorld = new G4PVPlacement(
        nullptr,
        G4ThreeVector(),
        logicalWorld,
        fWorldName,
        nullptr,
        false, 0,
        checkOverlaps
    );

    // ============================================================================
    // scintillator
    // ============================================================================

    // solid & logical scintillator construction

    auto solidScintillator = new G4Box(
        fScintillatorName,
        fScintillatorHalfSize[0],
        fScintillatorHalfSize[1],
        fScintillatorHalfSize[2]
    );
    this->fLogicalScintillator = new G4LogicalVolume(
        solidScintillator,
        scintillatorMaterial,
        fScintillatorName
    );

    G4Transform3D scintillatorTransform(
        G4RotationMatrix(),
        G4ThreeVector(0.0)
    );

    // ============================================================================
    // acrylic light guide
    // ============================================================================

    // solid & logical acrylic light guide construction

    auto solidAcrylic = new G4Trd(
        fAcrylicName,
        fScintillatorHalfSize[0],
        fAcrylicLightGuideSiPMSideHalfSize[0],
        fScintillatorHalfSize[1],
        fAcrylicLightGuideSiPMSideHalfSize[1],
        0.5 * fAcrylicLightGuideLength
    );
    auto logicalAcrylic = new G4LogicalVolume(
        solidAcrylic,
        acrylicMaterial,
        fAcrylicName
    );

    G4Transform3D acrylicTransform(
        G4RotationMatrix(),
        G4ThreeVector(0.0, 0.0, fScintillatorHalfSize[2] + fAcrylicScintillatorGap + 0.5 * fAcrylicLightGuideLength)
    );

    // ============================================================================
    // gap between acrylic and scintillator
    // ============================================================================

    // solid & logical gap between acrylic and scintillator construction

    auto solidAcrylicScintillatorGap = new G4Box(
        fAcrylicScintillatorGapName,
        fScintillatorHalfSize[0],
        fScintillatorHalfSize[1],
        0.5 * fAcrylicScintillatorGap
    );
    auto logicalAcrylicScintillatorGap = new G4LogicalVolume(
        solidAcrylicScintillatorGap,
        siliconeOilMaterial,
        fAcrylicScintillatorGapName
    );

    G4Transform3D acrylicScintillatorGapTransform(
        G4RotationMatrix(),
        G4ThreeVector(0.0, 0.0, fScintillatorHalfSize[2] + 0.5 * fAcrylicScintillatorGap)
    );

    // ============================================================================
    // gap between cover and scintillator
    // ============================================================================

    auto solidRod = new G4Box(
        fTempName,
        fCoverHoleHalfWidth,
        fCoverHoleHalfWidth,
        fScintillatorHalfSize[2] + fAcrylicLightGuideLength
    );
    auto GenerateCover = [this, solidRod](G4double in, G4double out)->std::pair<G4VSolid*, G4Transform3D> {
        auto solidPart1 = new G4SubtractionSolid(
            fTempName,
            new G4Box(
                fTempName,
                fScintillatorHalfSize[0] + out,
                fScintillatorHalfSize[1] + out,
                fScintillatorHalfSize[2] + 0.5 * out + 0.5 * fAcrylicScintillatorGap
            ),
            new G4Box(
                fTempName,
                fScintillatorHalfSize[0] + in,
                fScintillatorHalfSize[1] + in,
                fScintillatorHalfSize[2] + 0.5 * out + 0.5 * fAcrylicScintillatorGap
            ),
            G4Transform3D(
                G4RotationMatrix(),
                G4ThreeVector(0.0, 0.0, out - in)
            )
        );
        constexpr G4double ex = 1 * mm;
        auto solidPart2 = new G4SubtractionSolid(
            fTempName,
            new G4SubtractionSolid(
                fTempName,
                new G4Trd(
                    fTempName,
                    fScintillatorHalfSize[0] + out,
                    fAcrylicLightGuideSiPMSideHalfSize[0] + out -
                    (out / fAcrylicLightGuideLength)
                    * (fScintillatorHalfSize[0] - fAcrylicLightGuideSiPMSideHalfSize[0]),
                    fScintillatorHalfSize[1] + out,
                    fAcrylicLightGuideSiPMSideHalfSize[1] + out -
                    (out / fAcrylicLightGuideLength)
                    * (fScintillatorHalfSize[1] - fAcrylicLightGuideSiPMSideHalfSize[1]),
                    0.5 * (fAcrylicLightGuideLength + out)
                ),
                new G4Trd(
                    fTempName,
                    fScintillatorHalfSize[0] + in +
                    (ex / fAcrylicLightGuideLength)
                    * (fScintillatorHalfSize[0] - fAcrylicLightGuideSiPMSideHalfSize[0]),
                    fAcrylicLightGuideSiPMSideHalfSize[0] + in -
                    (in / fAcrylicLightGuideLength)
                    * (fScintillatorHalfSize[0] - fAcrylicLightGuideSiPMSideHalfSize[0]),
                    fScintillatorHalfSize[1] + in +
                    (ex / fAcrylicLightGuideLength)
                    * (fScintillatorHalfSize[0] - fAcrylicLightGuideSiPMSideHalfSize[0]),
                    fAcrylicLightGuideSiPMSideHalfSize[1] + in -
                    (in / fAcrylicLightGuideLength)
                    * (fScintillatorHalfSize[1] - fAcrylicLightGuideSiPMSideHalfSize[1]),
                    0.5 * (ex + fAcrylicLightGuideLength + in)
                ),
                G4Transform3D(
                    G4RotationMatrix(),
                    G4ThreeVector(0.0, 0.0, 0.5 * (in - ex - out))
                )
            ),
            solidRod
        );
        G4Transform3D part2Transform(
            G4RotationMatrix(),
            G4ThreeVector(0.0, 0.0,
                fScintillatorHalfSize[2] + 0.5 * out + 0.5 * fAcrylicScintillatorGap +
                0.5 * (fAcrylicLightGuideLength + out))
        );

        return std::make_pair(
            new G4UnionSolid(
                fTempName,
                solidPart1,
                solidPart2,
                part2Transform
            ),
            G4Transform3D(
                G4RotationMatrix(),
                G4ThreeVector(0.0, 0.0, 0.5 * (fAcrylicScintillatorGap - out))
            )
        );
    };

    // solid & logical gap between cover and scintillator construction

    auto&& coverScintillatorGapCollection = GenerateCover(0.0, fCoverScintillatorGap);
    coverScintillatorGapCollection.first->SetName(fCoverScintillatorGapName);
    auto logicalCoverScintillatorGap = new G4LogicalVolume(
        coverScintillatorGapCollection.first,
        gapMaterial,
        fCoverScintillatorGapName
    );
    G4Transform3D coverScintillatorGapTransform(coverScintillatorGapCollection.second);

    // ============================================================================
    // cover
    // ============================================================================

    // solid & logical cover construction

    auto&& coverCollection = GenerateCover(fCoverScintillatorGap, fCoverScintillatorGap + fCoverThickness);
    coverCollection.first->SetName(fCoverName);
    auto logicalCover = new G4LogicalVolume(
        coverCollection.first,
        coverMaterial,
        fCoverName
    );
    G4Transform3D coverTransform(coverCollection.second);

    // cover optical surface
    //
    auto coverOpticalSurface = new G4OpticalSurface(
        fCoverName + "_optical_surface",
        unified,
        polished,
        dielectric_metal
    );
    coverOpticalSurface->SetMaterialPropertiesTable(coverPropertiesTable);
    new G4LogicalSkinSurface(
        fCoverName + "_surface",
        logicalCover,
        coverOpticalSurface
    );

    // ============================================================================
    // SiPM
    // ============================================================================

    // solid & logical SiPM construction

    auto solidSiPM = new G4Box(
        fSiPMName,
        fSiPMHalfSize[0],
        fSiPMHalfSize[1],
        fSiPMHalfSize[2]
    );
    this->fLogicalSiPM = new G4LogicalVolume(
        solidSiPM,
        SiPMMaterial,
        fSiPMName
    );

    G4double SiPMZ = fScintillatorHalfSize[2] + fAcrylicScintillatorGap
        + fAcrylicLightGuideLength + fSiPMScintillatorGap + fSiPMHalfSize[2];
    G4Transform3D SiPMTransform(
        G4RotationMatrix(),
        G4ThreeVector(0.0, 0.0, SiPMZ)
    );

    // ============================================================================
    // light guides
    // ============================================================================

    // solid & logical light guide construction
    auto solidLightGuide = new G4SubtractionSolid(
        fSiliconeOilName,
        new G4Box(
            fTempName,
            fSiliconeOilHalfWidth,
            fSiliconeOilHalfWidth,
            0.5 * fSiliconeOilThickness
        ),
        solidSiPM,
        nullptr,
        G4ThreeVector(0.0, 0.0, fSiPMScintillatorGap - (0.5 * fSiliconeOilThickness - fSiPMHalfSize[2]))
    );
    auto logicalLightGuide = new G4LogicalVolume(
        solidLightGuide,
        siliconeOilMaterial,
        fSiliconeOilName
    );

    G4double lightGuideZ = fScintillatorHalfSize[2] + fAcrylicScintillatorGap
        + fAcrylicLightGuideLength + 0.5 * fSiliconeOilThickness;
    G4Transform3D lightGuideTransform(
        G4RotationMatrix(),
        G4ThreeVector(0.0, 0.0, lightGuideZ)
    );

    // ============================================================================
    // PCB
    // ============================================================================

    // solid & logical PCB construction

    auto solidPCB = new G4Box(
        fPCBName,
        fPCBHalfSize[0],
        fPCBHalfSize[1],
        fPCBHalfSize[2]
    );
    auto logicalPCB = new G4LogicalVolume(
        solidPCB,
        PCBMaterial,
        fPCBName
    );

    G4double PCBZ = SiPMZ + fSiPMHalfSize[2] + fPCBHalfSize[2];
    G4Transform3D PCBTransform(
        G4RotationMatrix(),
        G4ThreeVector(0.0, 0.0, PCBZ)
    );

    // ============================================================================
    // physical detector
    // ============================================================================

    auto detector = new G4AssemblyVolume();

    // place scintillator
    detector->AddPlacedVolume(fLogicalScintillator, scintillatorTransform); ++fDetectorPartCount;
    // place SiPM
    detector->AddPlacedVolume(fLogicalSiPM, SiPMTransform); ++fDetectorPartCount;
    // place light guide
    detector->AddPlacedVolume(logicalLightGuide, lightGuideTransform); ++fDetectorPartCount;
    // place PCB
    detector->AddPlacedVolume(logicalPCB, PCBTransform); ++fDetectorPartCount;
    // place acrylic
    detector->AddPlacedVolume(logicalAcrylic, acrylicTransform); ++fDetectorPartCount;
    // place gap between acrylic and scintillator
    detector->AddPlacedVolume(logicalAcrylicScintillatorGap, acrylicScintillatorGapTransform); ++fDetectorPartCount;
    // place gap between cover and scintillator
    detector->AddPlacedVolume(logicalCoverScintillatorGap, coverScintillatorGapTransform); ++fDetectorPartCount;
    // place cover
    detector->AddPlacedVolume(logicalCover, coverTransform); ++fDetectorPartCount;

    auto upperDetectorTrans =
        G4Transform3D(G4RotationMatrix(), G4ThreeVector(0.0, 0.0, 0.5 * fDetectorDistance));
    detector->MakeImprint(logicalWorld, upperDetectorTrans);
    auto lowerDetectorTrans =
        G4Transform3D(G4RotationMatrix(G4ThreeVector(1.0), M_PI), G4ThreeVector(0.0, 0.0, -0.5 * fDetectorDistance));
    detector->MakeImprint(logicalWorld, lowerDetectorTrans);

    return physicalWorld;
}

void SBDetectorConstruction::ConstructSDandField() {
    auto SDManager = G4SDManager::GetSDMpointer();
    SBScintillatorSD* scintillatorSD = new SBScintillatorSD(fScintillatorName);
    SDManager->AddNewDetector(scintillatorSD);
    SetSensitiveDetector(fLogicalScintillator, scintillatorSD);
    SBSiPMSD* SiPMSD = new SBSiPMSD(fSiPMName);
    SDManager->AddNewDetector(SiPMSD);
    SetSensitiveDetector(fLogicalSiPM, SiPMSD);
}

void SBDetectorConstruction::SetScintillatorMaterialProperties(G4Material* scintillatorMaterial) const {
    auto scintillatorPropertiesTable = new G4MaterialPropertiesTable();
    auto scintillatorProperties(CreateMapFromCSV<std::string, G4double>("./datafiles/scintillator_properties.csv"));

    scintillatorPropertiesTable->AddProperty(
        "RINDEX",
        &scintillatorProperties["RINDEX_energy"].front(),
        &scintillatorProperties["RINDEX"].front(),
        scintillatorProperties["RINDEX"].size()
    );
    scintillatorPropertiesTable->AddProperty(
        "ABSLENGTH",
        &scintillatorProperties["ABSLENGTH_energy"].front(),
        &scintillatorProperties["ABSLENGTH"].front(),
        scintillatorProperties["ABSLENGTH"].size()
    );
    scintillatorPropertiesTable->AddProperty(
        "RAYLEIGH",
        &scintillatorProperties["RAYLEIGH_energy"].front(),
        &scintillatorProperties["RAYLEIGH"].front(),
        scintillatorProperties["RAYLEIGH"].size()
    );
    scintillatorPropertiesTable->AddProperty(
        "FASTCOMPONENT",
        &scintillatorProperties["FASTCOMPONENT_energy"].front(),
        &scintillatorProperties["FASTCOMPONENT"].front(),
        scintillatorProperties["FASTCOMPONENT"].size()
    );
    scintillatorPropertiesTable->AddProperty(
        "SLOWCOMPONENT",
        &scintillatorProperties["SLOWCOMPONENT_energy"].front(),
        &scintillatorProperties["SLOWCOMPONENT"].front(),
        scintillatorProperties["SLOWCOMPONENT"].size()
    );
    scintillatorPropertiesTable->AddProperty(
        "REEMISSIONPROB",
        &scintillatorProperties["REEMISSIONPROB_energy"].front(),
        &scintillatorProperties["REEMISSIONPROB"].front(),
        scintillatorProperties["REEMISSIONPROB"].size()
    );
    scintillatorPropertiesTable->
        AddConstProperty("SCINTILLATIONYIELD", scintillatorProperties["SCINTILLATIONYIELD"].front());
    scintillatorPropertiesTable->
        AddConstProperty("RESOLUTIONSCALE", scintillatorProperties["RESOLUTIONSCALE"].front());
    scintillatorPropertiesTable->
        AddConstProperty("FASTTIMECONSTANT", scintillatorProperties["FASTTIMECONSTANT"].front());
    scintillatorPropertiesTable->
        AddConstProperty("SLOWTIMECONSTANT", scintillatorProperties["SLOWTIMECONSTANT"].front());
    scintillatorPropertiesTable->
        AddConstProperty("YIELDRATIO", scintillatorProperties["YIELDRATIO"].front());
    scintillatorPropertiesTable->
        AddConstProperty("AlphaFASTTIMECONSTANT", scintillatorProperties["AlphaFASTTIMECONSTANT"].front());
    scintillatorPropertiesTable->
        AddConstProperty("AlphaSLOWTIMECONSTANT", scintillatorProperties["AlphaSLOWTIMECONSTANT"].front());
    scintillatorPropertiesTable->
        AddConstProperty("AlphaYIELDRATIO", scintillatorProperties["AlphaYIELDRATIO"].front());
    scintillatorPropertiesTable->
        AddConstProperty("NeutronFASTTIMECONSTANT", scintillatorProperties["NeutronFASTTIMECONSTANT"].front());
    scintillatorPropertiesTable->
        AddConstProperty("NeutronSLOWTIMECONSTANT", scintillatorProperties["NeutronSLOWTIMECONSTANT"].front());
    scintillatorPropertiesTable->
        AddConstProperty("NeutronYIELDRATIO", scintillatorProperties["NeutronYIELDRATIO"].front());
    scintillatorPropertiesTable->
        AddConstProperty("ReemissionFASTTIMECONSTANT", scintillatorProperties["ReemissionFASTTIMECONSTANT"].front());
    scintillatorPropertiesTable->
        AddConstProperty("ReemissionSLOWTIMECONSTANT", scintillatorProperties["ReemissionSLOWTIMECONSTANT"].front());
    scintillatorPropertiesTable->
        AddConstProperty("ReemissionYIELDRATIO", scintillatorProperties["ReemissionYIELDRATIO"].front());

    scintillatorMaterial->SetMaterialPropertiesTable(scintillatorPropertiesTable);

    scintillatorMaterial->GetIonisation()->SetBirksConstant(0.15 * mm / MeV);
}

