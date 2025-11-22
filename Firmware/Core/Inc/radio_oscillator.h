/*
 * radio_oscillator.h - High-level Radio Oscillator Management
 *
 * This module provides a high-level abstraction for managing DDS oscillators
 * in a radio transceiver system. It handles VFO, BFO, LO, and modulation.
 */

#ifndef INC_RADIO_OSCILLATOR_H_
#define INC_RADIO_OSCILLATOR_H_

#include "oscillator.h"
#include <stdint.h>
#include <stdbool.h>

/* Radio Mode Definitions */
typedef enum {
    RADIO_MODE_USB = 0,      // Upper Sideband
    RADIO_MODE_LSB,          // Lower Sideband
    RADIO_MODE_CW,           // Continuous Wave (Morse)
    RADIO_MODE_AM,           // Amplitude Modulation
    RADIO_MODE_FM,           // Frequency Modulation
    RADIO_MODE_DATA,         // Digital modes (PSK, FT8, etc.)
} RadioMode;

/* Radio Band Definitions */
typedef enum {
    BAND_160M = 0,
    BAND_80M,
    BAND_40M,
    BAND_30M,
    BAND_20M,
    BAND_17M,
    BAND_15M,
    BAND_12M,
    BAND_10M,
    BAND_6M,
    BAND_CUSTOM
} RadioBand;

/* VFO Selection */
typedef enum {
    VFO_A = 0,
    VFO_B
} VFO_Select;

/* TX/RX State */
typedef enum {
    RADIO_RX = 0,
    RADIO_TX
} RadioState;

/* Band Information */
typedef struct {
    RadioBand band;
    uint32_t freq_min;
    uint32_t freq_max;
    uint32_t default_freq;
    const char *name;
} BandInfo;

/* Radio Oscillator System */
typedef struct {
    /* VFO instances */
    AD9834_Instance *vfo_a;
    AD9834_Instance *vfo_b;
    VFO_Select active_vfo;

    /* Local Oscillator */
    AD9834_Instance *lo;
    uint32_t if_frequency;

    /* BFO for SSB/CW */
    AD9834_Instance *bfo;
    uint32_t bfo_usb_offset;
    uint32_t bfo_lsb_offset;
    uint32_t bfo_cw_offset;

    /* Current state */
    RadioMode mode;
    RadioBand band;
    RadioState state;

    uint32_t frequency;           // Current operating frequency
    uint32_t step_size;           // Tuning step size (Hz)
    int32_t rit_offset;           // RX Incremental Tuning (Hz)
    int32_t xit_offset;           // TX Incremental Tuning (Hz)

    /* Settings */
    uint32_t tx_ramp_time_ms;     // Key-up/down ramp time
    uint32_t tune_ramp_time_ms;   // VFO tuning ramp time
    bool split_mode;              // Split TX/RX operation

    /* Calibration */
    int32_t frequency_correction; // Frequency calibration offset (Hz)

} RadioOscillator;

/* Function Prototypes */

/* Initialization */
RadioOscillator* RadioOsc_Create(SPI_HandleTypeDef *hspi,
                                 GPIO_TypeDef *vfo_a_port, uint16_t vfo_a_pin,
                                 GPIO_TypeDef *vfo_b_port, uint16_t vfo_b_pin,
                                 GPIO_TypeDef *lo_port, uint16_t lo_pin,
                                 GPIO_TypeDef *bfo_port, uint16_t bfo_pin);
void RadioOsc_Destroy(RadioOscillator *radio);
void RadioOsc_Init(RadioOscillator *radio, uint32_t if_freq);

/* VFO Control */
void RadioOsc_SetFrequency(RadioOscillator *radio, uint32_t frequency);
void RadioOsc_TuneUp(RadioOscillator *radio);
void RadioOsc_TuneDown(RadioOscillator *radio);
void RadioOsc_SetStepSize(RadioOscillator *radio, uint32_t step_hz);
uint32_t RadioOsc_GetFrequency(RadioOscillator *radio);

/* VFO A/B Management */
void RadioOsc_SelectVFO(RadioOscillator *radio, VFO_Select vfo);
void RadioOsc_SwapVFO(RadioOscillator *radio);
void RadioOsc_CopyVFO(RadioOscillator *radio);  // Copy active to inactive
VFO_Select RadioOsc_GetActiveVFO(RadioOscillator *radio);

/* Mode Control */
void RadioOsc_SetMode(RadioOscillator *radio, RadioMode mode);
RadioMode RadioOsc_GetMode(RadioOscillator *radio);

/* Band Control */
void RadioOsc_SetBand(RadioOscillator *radio, RadioBand band);
RadioBand RadioOsc_GetBand(RadioOscillator *radio);
const BandInfo* RadioOsc_GetBandInfo(RadioBand band);

/* TX/RX Control */
void RadioOsc_SetTX(RadioOscillator *radio);
void RadioOsc_SetRX(RadioOscillator *radio);
RadioState RadioOsc_GetState(RadioOscillator *radio);

/* RIT/XIT (Incremental Tuning) */
void RadioOsc_SetRIT(RadioOscillator *radio, int32_t offset_hz);
void RadioOsc_SetXIT(RadioOscillator *radio, int32_t offset_hz);
void RadioOsc_ClearRIT(RadioOscillator *radio);
void RadioOsc_ClearXIT(RadioOscillator *radio);

/* Split Operation */
void RadioOsc_EnableSplit(RadioOscillator *radio, bool enable);
bool RadioOsc_IsSplitEnabled(RadioOscillator *radio);

/* Modulation (CW, FSK, etc.) */
void RadioOsc_KeyDown(RadioOscillator *radio);  // CW key down
void RadioOsc_KeyUp(RadioOscillator *radio);    // CW key up
void RadioOsc_SendFSK(RadioOscillator *radio, bool mark); // FSK modulation
void RadioOsc_SetCWOffset(RadioOscillator *radio, uint32_t offset_hz);

/* Calibration */
void RadioOsc_SetFrequencyCorrection(RadioOscillator *radio, int32_t correction_hz);

/* Update Loop */
void RadioOsc_Update(RadioOscillator *radio);  // Call periodically for ramping

#endif /* INC_RADIO_OSCILLATOR_H_ */
