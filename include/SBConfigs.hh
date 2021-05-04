#ifndef SB_CONFIGS_H
#define SB_CONFIGS_H 1

// Result saving type.
// 0: Save as ROOT.
// 1: Save as csv.
#define SB_SAVE_TYPE                          1

#if SB_SAVE_TYPE == 0
#define SB_SAVE_SIPM_MAX_RESPONSE             true
#define SB_SAVE_PHOTON_HIT_INFO               true
#define SB_SAVE_SIPM_RESPONSE_WAVEFORM        true
#define SB_SAVE_ANYTHING (SB_SAVE_PHOTON_HIT_INFO || SB_SAVE_SIPM_RESPONSE_WAVEFORM || SB_SAVE_SIPM_MAX_RESPONSE)
#elif SB_SAVE_TYPE == 1
#define SB_SAVE_SIPM_MAX_RESPONSE             true
#define SB_SAVE_ANYTHING SB_SAVE_SIPM_MAX_RESPONSE
#endif

#endif

