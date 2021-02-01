#ifndef SB_CONFIGS_H
#define SB_CONFIGS_H 1

// Compile-time executed feature settings.
// 0 = Disable, 1 = enable.

//
// Using time for random seed if enabled.
#define SB_USING_TIME_RANDOM_SEED                0
//
// Enable optical physics. (Scintillation process, reflection, etc.)
// If only care about hit, this can be disabled.
#define SB_ENABLE_OPTICAL_PHYSICS                0
//
// Enable reflection on aluminum foil's surface.
#define SB_ENABLE_AL_FOIL_REFLECTION             1
//
// If enabled, scintillation light propagation will be supressed
// by setting absorption length at 0, which is unreal but
// will accelerate computation when you don't care about light propagation.
#define SB_SUPPRESS_SCINTILLATION_PROPAGATION    0

#endif

