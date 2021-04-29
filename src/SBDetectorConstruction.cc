#include "SBDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"
#include "G4AssemblyVolume.hh"

SBDetectorConstruction::SBDetectorConstruction() :
    fScintillatorHalfSize{ 5 * cm, 5 * cm, 0.5 * cm },
    fScintillatorName("scintillator"),

    fWorldName("world"),

    fCoverThickness(100 * um),
#if SB_STRUCTURE == 0 || SB_STRUCTURE == 2
    fCoverHoleHalfWidth(4 * mm),
#elif SB_STRUCTURE == 1 || SB_STRUCTURE == 3
    fCoverHoleHalfWidth(3 * mm),
#endif
    fGapWidth(50 * um),
    fCoverName("al_foil"),
    fGapName("gap"),

#if SB_STRUCTURE == 0
    fSiPMCount(1 * 2),
    fSiPMHalfSize{ 3.5 * mm, 3.5 * mm, 0.25 * mm },
#elif SB_STRUCTURE == 1 || SB_STRUCTURE == 3
    fSiPMCount(4),
    fSiPMHalfSize{ 0.25 * mm, 2.5 * mm, 2.5 * mm },
#elif SB_STRUCTURE == 2
    fSiPMCount(4),
    fSiPMHalfSize{ 3.5 * mm, 3.5 * mm, 0.25 * mm },
    fSiPMPositionList{
    G4ThreeVector(1.25 * cm, 1.25 * cm, fScintillatorHalfSize[2] + fSiPMScintillatorGap + fSiPMHalfSize[2]),
    G4ThreeVector(-1.25 * cm, 1.25 * cm, fScintillatorHalfSize[2] + fSiPMScintillatorGap + fSiPMHalfSize[2]),
    G4ThreeVector(-1.25 * cm, -1.25 * cm, fScintillatorHalfSize[2] + fSiPMScintillatorGap + fSiPMHalfSize[2]),
    G4ThreeVector(1.25 * cm, -1.25 * cm, fScintillatorHalfSize[2] + fSiPMScintillatorGap + fSiPMHalfSize[2]) },
#endif
    fSiPMScintillatorGap(30 * um),
    fSiPMName("SiPM"),

    fLightGuideHalfWidth(fCoverHoleHalfWidth),
    fLightGuideThickness(fGapWidth + fCoverThickness),
    fLightGuideName("light_guide"),
#if SB_STRUCTURE == 0 || SB_STRUCTURE == 2
    fPCBHalfSize{ 1.25 * cm, 1.25 * cm, 0.5 * mm },
#elif SB_STRUCTURE == 1 || SB_STRUCTURE == 3
    fPCBHalfSize{ 0.5 * mm, 1 * cm, 0.8 * cm },
#endif
    fPCBName("PCB"),

    fDetectorDistance(3 * cm),

    fTempName("_tmp"),

    fLogicalScintillator(nullptr),
    fLogicalSiPM(nullptr),

    fDetectorPartCount(1){}

SBDetectorConstruction::~SBDetectorConstruction() {}

G4VPhysicalVolume* SBDetectorConstruction::Construct() {
    // Set if check overlaps
    //
    constexpr G4bool checkOverlaps = true;

    // Construct materials.
    //
    G4PVDataVector energies, values; // Material properties.

    // worldRefractionIndex
    energies = { 1.0 * eV, 20.0 * eV };
    values = { 1.0, 1.0 };
    auto worldRefractionIndex = new G4MaterialPropertyVector(&energies[0], &values[0], energies.size());

    // coverReflectivity
    energies = { 1.0 * eV, 20.0 * eV };
    values = { 0.95, 0.95 };
    auto coverReflectivity = new G4MaterialPropertyVector(&energies[0], &values[0], energies.size());

    // gapRefractionIndex
    energies = { 1.587 * eV, 3.095 * eV };
    values = { 1.36, 1.36 };
    auto gapRefractionIndex = new G4MaterialPropertyVector(&energies[0], &values[0], energies.size());

    // lightGuideRefractionIndex
    energies = { 1.587 * eV, 3.095 * eV };
    values = { 1.403, 1.406 };
    auto lightGuideRefractionIndex = new G4MaterialPropertyVector(&energies[0], &values[0], energies.size());

    // NIST manager.
    G4NistManager* nist = G4NistManager::Instance();

    // world material
    //
    auto worldMaterial = nist->BuildMaterialWithNewDensity("void", "G4_AIR", 1e-24 * g / cm3);
    auto worldPropertiesTable = new G4MaterialPropertiesTable();
    worldPropertiesTable->AddProperty("RINDEX", worldRefractionIndex);
    worldMaterial->SetMaterialPropertiesTable(worldPropertiesTable);

    // scintillator material
    //
    auto scintillatorMaterial = new G4Material("plastic_scintillator", 1.032 * g / cm3, 2, kStateSolid);
    auto H = nist->FindOrBuildElement("H");
    auto C = nist->FindOrBuildElement("C");
    scintillatorMaterial->AddElement(C, 9);
    scintillatorMaterial->AddElement(H, 10);
    SetScintillatorMaterialProperties(scintillatorMaterial);

    // cover material
    //
    auto coverMaterial = nist->FindOrBuildMaterial("G4_Al");
    auto coverPropertiesTable = new G4MaterialPropertiesTable();
    coverPropertiesTable->AddProperty("REFLECTIVITY", coverReflectivity);
    // coverPropertiesTable set for optical surface.

    // gap material
    //
    auto gapMaterial = nist->FindOrBuildMaterial("G4_AIR");
    auto gapPropertiesTable = new G4MaterialPropertiesTable();
    gapPropertiesTable->AddProperty("RINDEX", gapRefractionIndex);
    gapMaterial->SetMaterialPropertiesTable(gapPropertiesTable);

    // SiPM material
    //
    auto SiPMMaterial = nist->FindOrBuildMaterial("G4_Si");
    auto SiPMPropertiesTable = new G4MaterialPropertiesTable();
    SiPMPropertiesTable->AddProperty("RINDEX", lightGuideRefractionIndex);
    G4double absorptionLengthPhotonEnergy[2] = { 1.0 * eV, 20.0 * eV };
    G4double absorptionLength[2] = { 0.0, 0.0 };
    SiPMPropertiesTable->AddProperty("ABSLENGTH", absorptionLengthPhotonEnergy, absorptionLength, 2);
    SiPMMaterial->SetMaterialPropertiesTable(SiPMPropertiesTable);

    // light guide materials
    // 
    auto lightGuideMaterial = new G4Material("silicone_oil", 0.97 * g / cm3, 4, kStateLiquid);
    auto O = nist->FindOrBuildElement("O");
    auto Si = nist->FindOrBuildElement("Si");
    lightGuideMaterial->AddElement(C, 2);
    lightGuideMaterial->AddElement(H, 6);
    lightGuideMaterial->AddElement(Si, 1);
    lightGuideMaterial->AddElement(O, 1);
    auto lightGuidePropertiesTable = new G4MaterialPropertiesTable();
    lightGuidePropertiesTable->AddProperty("RINDEX", lightGuideRefractionIndex);
    lightGuideMaterial->SetMaterialPropertiesTable(lightGuidePropertiesTable);

    // PCB material
    //
    G4Material* PCBMaterial = nist->FindOrBuildMaterial("G4_POLYCARBONATE");

    // ============================================================================
    // world
    // ============================================================================

    // world construction

    fWorldZMax = 0.5 * fDetectorDistance + 2.0 * fScintillatorHalfSize[2] + 1 * cm;
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
    G4VPhysicalVolume* physicalWorld = new G4PVPlacement(
        nullptr,
        G4ThreeVector(),
        logicalWorld,
        fWorldName,
        nullptr,
        false, 0,
        checkOverlaps
    );

    // ============================================================================
    // scintillators
    // ============================================================================

    // solid & logical scintillator construction

    G4VSolid* solidScintillator = new G4Box(
        fScintillatorName,
        fScintillatorHalfSize[0],
        fScintillatorHalfSize[1],
        fScintillatorHalfSize[2]
    );
#if SB_STRUCTURE == 3
    constexpr G4double tubRadius = 2 * mm;
    constexpr G4double tubXBegin = -4 * cm;
    constexpr G4double tubXEnd = 4 * cm;
    constexpr G4double tubXNum = 5;
    constexpr G4double tubYBegin = -4 * cm;
    constexpr G4double tubYEnd = 4 * cm;
    constexpr G4double tubYNum = 5;
    auto solidTub = new G4Tubs(
        fTempName,
        0.0, tubRadius,
        1 * m,
        0.0, 360 * deg
    );
    for (size_t i = 0; i < tubXNum; ++i) {
        for (size_t j = 0; j < tubYNum; ++j) {
            solidScintillator = new G4SubtractionSolid(
                fTempName,
                solidScintillator,
                solidTub,
                nullptr,
                G4ThreeVector(
                    tubXBegin + i * (tubXEnd - tubXBegin) / (tubXNum - 1),
                    tubYBegin + j * (tubYEnd - tubYBegin) / (tubYNum - 1)
                )
            );
        }
    }
    solidScintillator->SetName(fScintillatorName);
#endif
    this->fLogicalScintillator = new G4LogicalVolume(
        solidScintillator,
        scintillatorMaterial,
        fScintillatorName
    );

    // ============================================================================
    // cover and gap
    // ============================================================================

    // solid & logical cover construction

    auto solidCoverGapScintillator = new G4Box(
        fTempName,
        fScintillatorHalfSize[0] + fGapWidth + fCoverThickness,
        fScintillatorHalfSize[1] + fGapWidth + fCoverThickness,
        fScintillatorHalfSize[2] + fGapWidth + fCoverThickness
    );
    auto solidGapScintillator = new G4Box(
        fTempName,
        fScintillatorHalfSize[0] + fGapWidth,
        fScintillatorHalfSize[1] + fGapWidth,
        fScintillatorHalfSize[2] + fGapWidth
    );
    auto solidCoverWithoutHole = new G4SubtractionSolid(
        fCoverName + "_without_hole",
        solidCoverGapScintillator,
        solidGapScintillator
    );
    auto solidGapWithoutHole = new G4SubtractionSolid(
        fGapName + "_without_hole",
        solidGapScintillator,
        new G4Box(
            fTempName,
            fScintillatorHalfSize[0],
            fScintillatorHalfSize[1],
            fScintillatorHalfSize[2]
        )
    );
#if SB_STRUCTURE == 0
    auto solidHole = new G4Box(
        "hole",
        fCoverHoleHalfWidth,
        fCoverHoleHalfWidth,
        fGapWidth + fCoverThickness
    );
    const G4double holeZ = fScintillatorHalfSize[2];
    auto solidCover = new G4SubtractionSolid(
        fCoverName,
        solidCoverWithoutHole,
        solidHole,
        G4Transform3D(
            G4RotationMatrix(),
            G4ThreeVector(0, 0, holeZ)
        )
    );
    auto solidGap = new G4SubtractionSolid(
        fCoverName,
        solidGapWithoutHole,
        solidHole,
        G4Transform3D(
            G4RotationMatrix(),
            G4ThreeVector(0, 0, holeZ)
        )
    );
#elif SB_STRUCTURE == 1 || SB_STRUCTURE == 3
    auto solidHole = new G4Box(
        "hole",
        fGapWidth + fCoverThickness,
        fCoverHoleHalfWidth,
        fCoverHoleHalfWidth
    );
    const G4double holeX = fScintillatorHalfSize[0];
    const G4double holeY = fScintillatorHalfSize[1];
    auto solidCover = new G4SubtractionSolid(
        fCoverName,
        new G4SubtractionSolid(
            fTempName,
            new G4SubtractionSolid(
                fTempName,
                new G4SubtractionSolid(
                    fTempName,
                    solidCoverWithoutHole,
                    solidHole,
                    nullptr,
                    G4ThreeVector(holeX, 0, 0)
                ),
                solidHole,
                nullptr,
                G4ThreeVector(-holeX, 0, 0)
            ),
            solidHole,
            G4Transform3D(
                G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 90 * deg),
                G4ThreeVector(0, holeY, 0)
            )
        ),
        solidHole,
        G4Transform3D(
            G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 90 * deg),
            G4ThreeVector(0, -holeY, 0)
        )
    );
    auto solidGap = new G4SubtractionSolid(
        fGapName,
        new G4SubtractionSolid(
            fTempName,
            new G4SubtractionSolid(
                fTempName,
                new G4SubtractionSolid(
                    fTempName,
                    solidGapWithoutHole,
                    solidHole,
                    nullptr,
                    G4ThreeVector(holeX, 0, 0)
                ),
                solidHole,
                nullptr,
                G4ThreeVector(-holeX, 0, 0)
            ),
            solidHole,
            G4Transform3D(
                G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 90 * deg),
                G4ThreeVector(0, holeY, 0)
            )
        ),
        solidHole,
        G4Transform3D(
            G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 90 * deg),
            G4ThreeVector(0, -holeY, 0)
        )
    );
#elif SB_STRUCTURE == 2
    auto solidHole = new G4Box(
        "hole",
        fCoverHoleHalfWidth,
        fCoverHoleHalfWidth,
        fGapWidth + fCoverThickness
    );
    const G4double holeZ = fScintillatorHalfSize[2];
    auto solidCover = new G4SubtractionSolid(
        fCoverName,
        new G4SubtractionSolid(
            fTempName,
            new G4SubtractionSolid(
                fTempName,
                new G4SubtractionSolid(
                    fTempName,
                    solidCoverWithoutHole,
                    solidHole,
                    G4Transform3D(
                        G4RotationMatrix(),
                        G4ThreeVector(fSiPMPositionList[0].x(), fSiPMPositionList[0].y(), holeZ)
                    )
                ),
                solidHole,
                G4Transform3D(
                    G4RotationMatrix(),
                    G4ThreeVector(fSiPMPositionList[1].x(), fSiPMPositionList[1].y(), holeZ)
                )
            ),
            solidHole,
            G4Transform3D(
                G4RotationMatrix(),
                G4ThreeVector(fSiPMPositionList[2].x(), fSiPMPositionList[2].y(), holeZ)
            )
        ),
        solidHole,
        G4Transform3D(
            G4RotationMatrix(),
            G4ThreeVector(fSiPMPositionList[3].x(), fSiPMPositionList[3].y(), holeZ)
        )
    );
    auto solidGap = new G4SubtractionSolid(
        fCoverName,
        new G4SubtractionSolid(
            fTempName,
            new G4SubtractionSolid(
                fTempName,
                new G4SubtractionSolid(
                    fTempName,
                    solidGapWithoutHole,
                    solidHole,
                    G4Transform3D(
                        G4RotationMatrix(),
                        G4ThreeVector(fSiPMPositionList[0].x(), fSiPMPositionList[0].y(), holeZ)
                    )
                ),
                solidHole,
                G4Transform3D(
                    G4RotationMatrix(),
                    G4ThreeVector(fSiPMPositionList[1].x(), fSiPMPositionList[1].y(), holeZ)
                )
            ),
            solidHole,
            G4Transform3D(
                G4RotationMatrix(),
                G4ThreeVector(fSiPMPositionList[2].x(), fSiPMPositionList[2].y(), holeZ)
            )
        ),
        solidHole,
        G4Transform3D(
            G4RotationMatrix(),
            G4ThreeVector(fSiPMPositionList[3].x(), fSiPMPositionList[3].y(), holeZ)
        )
    );
#endif
    auto logicalCover = new G4LogicalVolume(
        solidCover,
        coverMaterial,
        fCoverName
    );
    auto logicalGap = new G4LogicalVolume(
        solidGap,
        gapMaterial,
        fGapName
    );

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

    // ============================================================================
    // light guides
    // ============================================================================

    // solid & logical light guide construction
#if SB_STRUCTURE == 0 || SB_STRUCTURE == 2
    auto solidLightGuide = new G4SubtractionSolid(
        fLightGuideName,
        new G4Box(
            fTempName,
            fLightGuideHalfWidth,
            fLightGuideHalfWidth,
            0.5 * fLightGuideThickness
        ),
        solidSiPM,
        nullptr,
        G4ThreeVector(0.0, 0.0, fSiPMScintillatorGap - (0.5 * fLightGuideThickness - fSiPMHalfSize[2]))
    );
#elif SB_STRUCTURE == 1 || SB_STRUCTURE == 3
    auto solidLightGuide = new G4SubtractionSolid(
        fLightGuideName,
        new G4Box(
            fTempName,
            0.5 * fLightGuideThickness,
            fLightGuideHalfWidth,
            fLightGuideHalfWidth
        ),
        solidSiPM,
        nullptr,
        G4ThreeVector(fSiPMScintillatorGap - (0.5 * fLightGuideThickness - fSiPMHalfSize[0]), 0, 0)
    );
#endif
    auto logicalLightGuide = new G4LogicalVolume(
        solidLightGuide,
        lightGuideMaterial,
        fLightGuideName
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

    // ============================================================================
    // physical detector
    // ============================================================================

    auto detector = new G4AssemblyVolume();

#if SB_STRUCTURE == 0
    //
    // place SiPM
    //
    G4double SiPMZ = fScintillatorHalfSize[2] + fSiPMScintillatorGap + fSiPMHalfSize[2];
    auto SiPMTransform = G4Transform3D(
        G4RotationMatrix(),
        G4ThreeVector(0.0, 0.0, SiPMZ)
    );
    detector->AddPlacedVolume(fLogicalSiPM, SiPMTransform); ++fDetectorPartCount;

    //
    // place light guide
    //
    G4double lightGuideZ = fScintillatorHalfSize[2] + 0.5 * fLightGuideThickness;
    auto lightGuideTransform = G4Transform3D(
        G4RotationMatrix(),
        G4ThreeVector(0.0, 0.0, lightGuideZ)
    );
    detector->AddPlacedVolume(logicalLightGuide, lightGuideTransform); ++fDetectorPartCount;

    //
    // place PCB
    //
    G4double PCBZ = SiPMZ + fSiPMHalfSize[2] + fPCBHalfSize[2];
    auto PCBTransform = G4Transform3D(
        G4RotationMatrix(),
        G4ThreeVector(0.0, 0.0, PCBZ)
    );
    detector->AddPlacedVolume(logicalPCB, PCBTransform); ++fDetectorPartCount;
#elif SB_STRUCTURE == 1 || SB_STRUCTURE == 3
    //
    // place SiPM
    //
    G4double SiPMX = fScintillatorHalfSize[0] + fSiPMScintillatorGap + fSiPMHalfSize[0];
    auto SiPMpXTransform = G4Transform3D(
        G4RotationMatrix(),
        G4ThreeVector(SiPMX, 0.0, 0.0)
    );
    detector->AddPlacedVolume(fLogicalSiPM, SiPMpXTransform); ++fDetectorPartCount;
    auto SiPMnXTransform = G4Transform3D(
        G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 180 * deg),
        G4ThreeVector(-SiPMX, 0.0, 0.0)
    );
    detector->AddPlacedVolume(fLogicalSiPM, SiPMnXTransform); ++fDetectorPartCount;

    G4double SiPMY = fScintillatorHalfSize[1] + fSiPMScintillatorGap + fSiPMHalfSize[0];
    auto SiPMpYTransform = G4Transform3D(
        G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 90 * deg),
        G4ThreeVector(0.0, SiPMY, 0.0)
    );
    detector->AddPlacedVolume(fLogicalSiPM, SiPMpYTransform); ++fDetectorPartCount;
    auto SiPMnYTransform = G4Transform3D(
        G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 270 * deg),
        G4ThreeVector(0.0, -SiPMY, 0.0)
    );
    detector->AddPlacedVolume(fLogicalSiPM, SiPMnYTransform); ++fDetectorPartCount;

    //
    // place light guide
    //
    G4double lightGuideX = fScintillatorHalfSize[0] + 0.5 * fLightGuideThickness;
    auto lightGuidePXTransform = G4Transform3D(
        G4RotationMatrix(),
        G4ThreeVector(lightGuideX, 0.0, 0.0)
    );
    detector->AddPlacedVolume(logicalLightGuide, lightGuidePXTransform); ++fDetectorPartCount;
    auto lightGuideNXTransform = G4Transform3D(
        G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 180 * deg),
        G4ThreeVector(-lightGuideX, 0.0, 0.0)
    );
    detector->AddPlacedVolume(logicalLightGuide, lightGuideNXTransform); ++fDetectorPartCount;

    G4double lightGuideY = fScintillatorHalfSize[1] + 0.5 * fLightGuideThickness;
    auto lightGuidePYTransform = G4Transform3D(
        G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 90 * deg),
        G4ThreeVector(0.0, lightGuideY, 0.0)
    );
    detector->AddPlacedVolume(logicalLightGuide, lightGuidePYTransform); ++fDetectorPartCount;
    auto lightGuideNYTransform = G4Transform3D(
        G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 270 * deg),
        G4ThreeVector(0.0, -lightGuideY, 0.0)
    );
    detector->AddPlacedVolume(logicalLightGuide, lightGuideNYTransform); ++fDetectorPartCount;

    //
    // place PCB
    //
    G4double PCBX = SiPMX + fSiPMHalfSize[0] + fPCBHalfSize[0];
    auto PCBPXTransform = G4Transform3D(
        G4RotationMatrix(),
        G4ThreeVector(PCBX, 0.0, 0.0)
    );
    detector->AddPlacedVolume(logicalPCB, PCBPXTransform); ++fDetectorPartCount;
    auto PCBNXTransform = G4Transform3D(
        G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 180 * deg),
        G4ThreeVector(-PCBX, 0.0, 0.0)
    );
    detector->AddPlacedVolume(logicalPCB, PCBNXTransform); ++fDetectorPartCount;

    G4double PCBY = SiPMY + fSiPMHalfSize[0] + fPCBHalfSize[0];
    auto PCBPYTransform = G4Transform3D(
        G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 90 * deg),
        G4ThreeVector(0.0, PCBY, 0.0)
    );
    detector->AddPlacedVolume(logicalPCB, PCBPYTransform); ++fDetectorPartCount;
    auto PCBNYTransform = G4Transform3D(
        G4RotationMatrix(G4ThreeVector(0.0, 0.0, 1.0), 270 * deg),
        G4ThreeVector(0.0, -PCBY, 0.0)
    );
    detector->AddPlacedVolume(logicalPCB, PCBNYTransform); ++fDetectorPartCount;
#elif SB_STRUCTURE == 2
    //
    // place SiPM
    //
    for (const auto& SiPMPosition : fSiPMPositionList) {
        auto SiPMTransform = G4Transform3D(
            G4RotationMatrix(),
            SiPMPosition
        );
        detector->AddPlacedVolume(fLogicalSiPM, SiPMTransform); ++fDetectorPartCount;
    }

    //
    // place light guide
    //
    for (const auto& SiPMPosition : fSiPMPositionList) {
        auto lightGuidePosition = G4ThreeVector(
            SiPMPosition.x(),
            SiPMPosition.y(),
            fScintillatorHalfSize[2] + 0.5 * fLightGuideThickness
        );
        auto lightGuideTransform = G4Transform3D(
            G4RotationMatrix(),
            lightGuidePosition
        );
        detector->AddPlacedVolume(logicalLightGuide, lightGuideTransform); ++fDetectorPartCount;
    }

    //
    // place PCB
    //
    for (const auto& SiPMPosition : fSiPMPositionList) {
        auto PCBPosition = SiPMPosition + G4ThreeVector(0, 0, fSiPMHalfSize[2] + fPCBHalfSize[2]);
        auto PCBTransform = G4Transform3D(
            G4RotationMatrix(),
            PCBPosition
        );
        detector->AddPlacedVolume(logicalPCB, PCBTransform); ++fDetectorPartCount;
    }
#endif

    //
    // place Scintillator & al foil & gap
    //
    auto noTransform = G4Transform3D();
    detector->AddPlacedVolume(fLogicalScintillator, noTransform); ++fDetectorPartCount;
    detector->AddPlacedVolume(logicalCover, noTransform); ++fDetectorPartCount;
    detector->AddPlacedVolume(logicalGap, noTransform); ++fDetectorPartCount;

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

