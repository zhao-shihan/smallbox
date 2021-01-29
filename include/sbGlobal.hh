#ifndef SB_GLOBAL_H_
#define SB_GLOBAL_H_ 1

#include <utility>
#include "G4SystemOfUnits.hh"
#include "G4String.hh"

constexpr G4double gWorldRadius = 0.3 * m;
static const G4String gWorldName("world");
static const G4String gWorldMaterialName("G4_AIR");

constexpr G4double gScintillatorHalfSize[3] = { 5.0 * cm, 5.0 * cm, 0.5 * cm };
constexpr G4double gScintillatorDistance = 1.0 * cm;
static const std::pair<G4String, G4String> gScintillatorsName("scintillator_1", "scintillator_2");
static const G4String gSensitiveDetectorName("scintillator");
static const G4String gScintillatorMaterialName("plastic_scintillator");

constexpr G4double gAlFoilThickness = 100 * um;
constexpr G4double gAlFoilHoleHalfWidth = 5 * mm;
constexpr G4double gAlFoilScintillatorGap = 50 * um;
static const std::pair<G4String, G4String> gAlFoilsName("al_foil_1", "al_foil_2");
static const G4String gAlFoilMaterialName("G4_Al");

#endif

