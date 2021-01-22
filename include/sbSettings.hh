#ifndef SB_SETTINGS_H
#define SB_SETTINGS_H 1

// Compile-time-executed feature settings.
// 0 = Disable, 1 = enable.

// Enable optical physics. (Scintillation process, reflection, etc.)
// If only care about hit, this can be disabled.
#define SB_ENABLE_OPTICAL_PHYSICS                1

#if SB_ENABLE_OPTICAL_PHYSICS
// Enable refraction & reflection on scintillator's surface.
// If disabled, the refraction index of scintillator will be set at 1.0
#define SB_ENABLE_SCINTILLATOR_REF               1

// Enable reflection on aluminum foil's surface.
#define SB_ENABLE_AL_FOIL_REFLECTION             1

// If enabled, scintillation light propagation will be supressed
// by setting absorption length at 0, which is unreal but
// will accelerate computation when you don't care about light propagation.
#define SB_SUPPRESS_SCINTILLATION_PROPAGATION    1
#endif

#endif

