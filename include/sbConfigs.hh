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
#define SB_ENABLE_OPTICAL_PHYSICS                1
//
// Enable reflection on aluminum foil's surface.
#define SB_ENABLE_AL_FOIL_REFLECTION             1
//
// If enabled, optical photons will be absorbed immediately
// when generated in scintillators.
#define SB_KILL_SCINTILLATION_PHOTON             0
//
// Process and save SiPM optical photon hit if enabled.
#define SB_PROCESS_SIPM_HIT                      1
//
// Process and save scintillator muon hit if enabled.
#define SB_PROCESS_SCINTILLATOR_HIT              1

#endif

