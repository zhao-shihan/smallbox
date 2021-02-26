#ifndef SB_GLOBAL_H
#define SB_GLOBAL_H 1

#include <utility>
#include <cmath>
#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"

//
// Batch flag

extern G4bool gRunningInBatch;

//
// Cosmic muon source

constexpr G4double gMaxE_GeV = 5000;
constexpr G4double gEffectiveRange = 0.25 * m;
constexpr G4double gSphereRadius = 2.828427 * gEffectiveRange;

//
// Detector construction

typedef std::pair<G4String, G4String> G4StringPair;
typedef std::pair<G4ThreeVector, G4ThreeVector> G4ThreeVectorPair;

// World

constexpr G4double gWorldRadius = 1.414214 * gEffectiveRange + gSphereRadius + 1.0;
constexpr G4double gWorldHalfHeight = gSphereRadius + 1.0;

static const G4String gWorldName("world");
static const G4String gWorldMaterialName("G4_AIR");

// Scintillator

constexpr G4double gScintillatorHalfSize[3] = { 5.0 * cm, 5.0 * cm, 0.5 * cm };
constexpr G4double gScintillatorDistance = 1.5 * cm;
constexpr G4double gUpperScintillatorzPosition = 0.5 * gScintillatorDistance + gScintillatorHalfSize[2];
static const G4ThreeVectorPair gScintillatorsPosition(G4ThreeVector(0.0, 0.0, gUpperScintillatorzPosition), G4ThreeVector(0.0, 0.0, -gUpperScintillatorzPosition));

static const G4String gScintillatorGeneralName("scintillator");
static const G4StringPair gScintillatorsName("upper_scintillator", "lower_scintillator");
static const G4String gScintillatorSDName("scintillator");
static const G4String gScintillatorMaterialName("plastic_scintillator");

// Aluminum foil

constexpr G4double gAlFoilThickness = 100 * um;
constexpr G4double gAlFoilHoleHalfWidth = 5 * mm;
constexpr G4double gAlFoilScintillatorGap = 50 * um;

static const G4String gAlFoilGeneralName("al_foil");
static const G4StringPair gAlFoilsName("upper_al_foil", "lower_al_foil");
static const G4String gAlFoilMaterialName("G4_Al");

// SiPM

constexpr G4double gSiPMHalfSize[3] = { 4 * mm, 4 * mm, 0.25 * mm };
constexpr G4double gSiPMScintillatorGap = 50 * um;
constexpr G4double gUpperSiPMzPosition = gUpperScintillatorzPosition + gScintillatorHalfSize[2] + gSiPMScintillatorGap + gSiPMHalfSize[2];
static const G4ThreeVectorPair gSiPMsPosition(G4ThreeVector(0.0, 0.0, gUpperSiPMzPosition), G4ThreeVector(0.0, 0.0, -gUpperSiPMzPosition));

static const G4String gSiPMGeneralName("SiPM");
static const G4StringPair gSiPMsName("upper_SiPM", "lower_SiPM");
static const G4String gSiPMSDName("SiPM");
static const G4String gSiPMMaterialName("G4_Si");

// Light guide

constexpr G4double gLightGuideHalfWidth = gAlFoilHoleHalfWidth;
constexpr G4double gLightGuideThickness = gAlFoilScintillatorGap + gAlFoilThickness;
constexpr G4double gUpperLightGuidezPosition = gUpperScintillatorzPosition + gScintillatorHalfSize[2] + 0.5 * gLightGuideThickness;
static const G4ThreeVectorPair gLightGuidesPosition(G4ThreeVector(0.0, 0.0, gUpperLightGuidezPosition), G4ThreeVector(0.0, 0.0, -gUpperLightGuidezPosition));

static const G4String gLightGuideGeneralName("light_guide");
static const G4StringPair gLightGuidesName("upper_light_guide", "lower_light_guide");
static const G4String gLightGuideMaterialName("silicone_oil");

// PCB

constexpr G4double gPCBHalfSize[3] = { 2.5 * cm, 2.5 * cm, 0.5 * mm };
constexpr G4double gUpperPCBzPosition = gUpperSiPMzPosition + gSiPMHalfSize[2] + gPCBHalfSize[2];
static const G4ThreeVectorPair gPCBsPosition(G4ThreeVector(0.0, 0.0, gUpperPCBzPosition), G4ThreeVector(0.0, 0.0, -gUpperPCBzPosition));

static const G4String gPCBGeneralName("PCB");
static const G4StringPair gPCBsName("upper_PCB", "lower_PCB");
static const G4String gPCBMaterialName("G4_POLYCARBONATE");

//
// Analysis & file io

static const G4String gRootFileName("smallbox");
static const G4String gSiPMResultCSVDestDir("SiPMresponse");

#endif

