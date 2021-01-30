#ifndef SB_GLOBAL_H
#define SB_GLOBAL_H 1

#include <utility>
#include "G4SystemOfUnits.hh"
#include "G4String.hh"

constexpr G4double gWorldRadius = 0.3 * m;
static const G4String gWorldName("world");
static const G4String gWorldMaterialName("G4_AIR");

constexpr G4double gScintillatorHalfSize[3] = { 5.0 * cm, 5.0 * cm, 0.5 * cm };
constexpr G4double gScintillatorDistance = 1.0 * cm;
constexpr G4double gDistanceBetweenScintillatorAndWorldBottom = 5.0 * cm;
static const std::pair<G4String, G4String> gScintillatorsName("scintillator_1", "scintillator_2");
static const G4String gScintillatorSDName("scintillator");
static const G4String gScintillatorMaterialName("plastic_scintillator");

constexpr G4double gAlFoilThickness = 100 * um;
constexpr G4double gAlFoilHoleHalfWidth = 5 * mm;
constexpr G4double gAlFoilScintillatorGap = 50 * um;
static const std::pair<G4String, G4String> gAlFoilsName("al_foil_1", "al_foil_2");
static const G4String gAlFoilMaterialName("G4_Al");

constexpr G4double gSiPMHalfSize[3] = { 4 * mm, 4 * mm, 0.25 * mm };
static const std::pair<G4String, G4String> gSiPMsName("SiPM_1", "SiPM_2");
static const G4String gSiPMSDName("SiPM");
static const G4String gSiPMMaterialName("G4_Si");

constexpr G4double gPCBHalfSize[3] = { 2.5 * cm, 2.5 * cm, 0.5 * mm };
static const std::pair<G4String, G4String> gPCBsName("PCB_1", "PCB_2");
static const G4String gPCBMaterialName("G4_POLYCARBONATE");

#endif

