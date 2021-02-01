#ifndef SB_GLOBAL_H
#define SB_GLOBAL_H 1

#include <utility>
#include <cmath>
#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"

//
// Cosmic muon source

constexpr G4double gEffectiveRange = 0.25 * m;
constexpr G4double gSphereRadius = 2.828427 * gEffectiveRange;

//
// Detector construction

// World

constexpr G4double gWorldRadius = 1.414214 * gEffectiveRange + gSphereRadius + 1.0;
constexpr G4double gWorldHalfHeight = gSphereRadius + 1.0;

static const G4String gWorldName("world");
static const G4String gWorldMaterialName("G4_AIR");

// Scintillator

constexpr G4double gScintillatorHalfSize[3] = { 5.0 * cm, 5.0 * cm, 0.5 * cm };
constexpr G4double gScintillatorDistance = 1.0 * cm;
constexpr G4double gDistanceBetweenScintillatorAndWorldBottom = 5.0 * cm;
constexpr G4double gScintillator1zPosition = 0.5 * gScintillatorDistance + gScintillatorHalfSize[2];
static const std::pair<G4ThreeVector, G4ThreeVector> gScintillatorsPosition(G4ThreeVector(0.0, 0.0, gScintillator1zPosition), G4ThreeVector(0.0, 0.0, -gScintillator1zPosition));

static const std::pair<G4String, G4String> gScintillatorsName("scintillator_1", "scintillator_2");
static const G4String gScintillatorSDName("scintillator");
static const G4String gScintillatorMaterialName("plastic_scintillator");

// Aluminum foil

constexpr G4double gAlFoilThickness = 100 * um;
constexpr G4double gAlFoilHoleHalfWidth = 5 * mm;
constexpr G4double gAlFoilScintillatorGap = 50 * um;

static const std::pair<G4String, G4String> gAlFoilsName("al_foil_1", "al_foil_2");
static const G4String gAlFoilMaterialName("G4_Al");

// SiPM

constexpr G4double gSiPMHalfSize[3] = { 4 * mm, 4 * mm, 0.25 * mm };
constexpr G4double gSiPM1zPosition = 0.5 * gScintillatorDistance + 2.0 * gScintillatorHalfSize[2] + gSiPMHalfSize[2];
static const std::pair<G4ThreeVector, G4ThreeVector> gSiPMsPosition(G4ThreeVector(0.0, 0.0, gSiPM1zPosition), G4ThreeVector(0.0, 0.0, -gSiPM1zPosition));

static const std::pair<G4String, G4String> gSiPMsName("SiPM_1", "SiPM_2");
static const G4String gSiPMSDName("SiPM");
static const G4String gSiPMMaterialName("G4_Si");

// PCB

constexpr G4double gPCBHalfSize[3] = { 2.5 * cm, 2.5 * cm, 0.5 * mm };
constexpr G4double gPCB1zPosition = 0.5 * gScintillatorDistance + 2.0 * gScintillatorHalfSize[2] + 2.0 * gSiPMHalfSize[2] + gPCBHalfSize[2];
static const std::pair<G4ThreeVector, G4ThreeVector> gPCBsPosition(G4ThreeVector(0.0, 0.0, gPCB1zPosition), G4ThreeVector(0.0, 0.0, -gPCB1zPosition));

static const std::pair<G4String, G4String> gPCBsName("PCB_1", "PCB_2");
static const G4String gPCBMaterialName("G4_POLYCARBONATE");

//
// Analysis

static const G4String gRootFileName("smallbox");

#endif

