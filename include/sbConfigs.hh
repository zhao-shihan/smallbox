#ifndef SB_CONFIGS_H
#define SB_CONFIGS_H 1

// Compile-time executed feature settings.
// 0 = Disable, 1 = enable.

//
// Using time for random seed if enabled.
#define SB_USING_TIME_RANDOM_SEED                false
//
// Enable optical physics. (Scintillation process, reflection, etc.)
// If only care about hit, this can be disabled.
#define SB_ENABLE_OPTICAL_PHYSICS                false
//
// Enable reflection on aluminum foil's surface.
#define SB_ENABLE_AL_FOIL_REFLECTION             true
//
// If enabled, optical photons will be absorbed immediately
// when generated in scintillators.
#define SB_KILL_SCINTILLATION_PHOTON             false
//
// Process and save SiPM optical photon hit if enabled.
#define SB_PROCESS_SIPM_HIT                      false
//
// Process and save scintillator muon hit if enabled.
#define SB_PROCESS_SCINTILLATOR_HIT              true

#endif

