#ifndef SB_SWITCH_H
#define SB_SWITCH_H 1

// Compile-time-executed feature settings.
// 0 = Disable, 1 = enable.

// Enable optical physics. (Scintillation process, reflection, etc.)
#define SB_ENABLE_OPTICAL_PHYSICS                1

#if SB_ENABLE_OPTICAL_PHYSICS
// Enable total reflection on scintillator's surface.
// ToDO #define SB_ENABLE_SCINTILLATOR_REFLECTION        1

// Enable reflection on aluminum foil's surface.
#define SB_ENABLE_AL_FOIL_REFLECTION             1
#endif

#endif

