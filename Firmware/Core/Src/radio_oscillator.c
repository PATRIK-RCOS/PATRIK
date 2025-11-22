/*
 * radio_oscillator.c - High-level Radio Oscillator Management Implementation
 */

#include "radio_oscillator.h"
#include <stdlib.h>
#include <string.h>

/* Band definitions */
static const BandInfo band_table[] = {
    {BAND_160M, 1800000,  2000000,  1850000,  "160m"},
    {BAND_80M,  3500000,  4000000,  3750000,  "80m"},
    {BAND_40M,  7000000,  7300000,  7074000,  "40m"},
    {BAND_30M,  10100000, 10150000, 10136000, "30m"},
    {BAND_20M,  14000000, 14350000, 14074000, "20m"},
    {BAND_17M,  18068000, 18168000, 18100000, "17m"},
    {BAND_15M,  21000000, 21450000, 21074000, "15m"},
    {BAND_12M,  24890000, 24990000, 24915000, "12m"},
    {BAND_10M,  28000000, 29700000, 28074000, "10m"},
    {BAND_6M,   50000000, 54000000, 50313000, "6m"},
    {BAND_CUSTOM, 0, 0xFFFFFFFF, 7074000, "Custom"}
};

/* Static instance (can support multiple radios if needed) */
static RadioOscillator radio_instance;

/* Private helper functions */
static void RadioOsc_UpdateLO(RadioOscillator *radio);
static void RadioOsc_UpdateBFO(RadioOscillator *radio);
static uint32_t RadioOsc_CalculateLOFreq(RadioOscillator *radio, uint32_t rf_freq);

/**
 * @brief Create radio oscillator system
 */
RadioOscillator* RadioOsc_Create(SPI_HandleTypeDef *hspi,
                                 GPIO_TypeDef *vfo_a_port, uint16_t vfo_a_pin,
                                 GPIO_TypeDef *vfo_b_port, uint16_t vfo_b_pin,
                                 GPIO_TypeDef *lo_port, uint16_t lo_pin,
                                 GPIO_TypeDef *bfo_port, uint16_t bfo_pin) {
    RadioOscillator *radio = &radio_instance;
    memset(radio, 0, sizeof(RadioOscillator));

    // Create DDS instances
    radio->vfo_a = AD9834_Create(hspi, vfo_a_port, vfo_a_pin, AD9834_MCLK);
    radio->vfo_b = AD9834_Create(hspi, vfo_b_port, vfo_b_pin, AD9834_MCLK);
    radio->lo = AD9834_Create(hspi, lo_port, lo_pin, AD9834_MCLK);
    radio->bfo = AD9834_Create(hspi, bfo_port, bfo_pin, AD9834_MCLK);

    if (!radio->vfo_a || !radio->vfo_b || !radio->lo || !radio->bfo) {
        return NULL;
    }

    // Set defaults
    radio->active_vfo = VFO_A;
    radio->mode = RADIO_MODE_USB;
    radio->band = BAND_40M;
    radio->state = RADIO_RX;
    radio->step_size = 100;  // 100 Hz steps
    radio->tx_ramp_time_ms = 10;
    radio->tune_ramp_time_ms = 50;

    return radio;
}

/**
 * @brief Destroy radio oscillator system
 */
void RadioOsc_Destroy(RadioOscillator *radio) {
    if (!radio) return;

    AD9834_Destroy(radio->vfo_a);
    AD9834_Destroy(radio->vfo_b);
    AD9834_Destroy(radio->lo);
    AD9834_Destroy(radio->bfo);

    memset(radio, 0, sizeof(RadioOscillator));
}

/**
 * @brief Initialize radio oscillator system
 */
void RadioOsc_Init(RadioOscillator *radio, uint32_t if_freq) {
    if (!radio) return;

    radio->if_frequency = if_freq;

    // Standard BFO offsets for different modes
    radio->bfo_usb_offset = 1500;   // USB: +1.5 kHz
    radio->bfo_lsb_offset = -1500;  // LSB: -1.5 kHz
    radio->bfo_cw_offset = 700;     // CW: +700 Hz

    // Initialize all DDS instances
    AD9834_Init(radio->vfo_a);
    AD9834_Init(radio->vfo_b);
    AD9834_Init(radio->lo);
    AD9834_Init(radio->bfo);

    // Set all to sine wave output
    AD9834_SetWaveform(radio->vfo_a, AD9834_WAVEFORM_SINE);
    AD9834_SetWaveform(radio->vfo_b, AD9834_WAVEFORM_SINE);
    AD9834_SetWaveform(radio->lo, AD9834_WAVEFORM_SINE);
    AD9834_SetWaveform(radio->bfo, AD9834_WAVEFORM_SINE);

    // Set initial frequency to default for band
    const BandInfo *band_info = RadioOsc_GetBandInfo(radio->band);
    RadioOsc_SetFrequency(radio, band_info->default_freq);

    // Start in RX mode
    RadioOsc_SetRX(radio);
}

/**
 * @brief Set operating frequency
 */
void RadioOsc_SetFrequency(RadioOscillator *radio, uint32_t frequency) {
    if (!radio) return;

    // Apply frequency correction
    frequency += radio->frequency_correction;

    radio->frequency = frequency;

    // Update the active VFO
    AD9834_Instance *active = (radio->active_vfo == VFO_A) ? radio->vfo_a : radio->vfo_b;

    if (radio->tune_ramp_time_ms > 0) {
        uint32_t current = AD9834_GetCurrentFrequency(active);
        if (frequency > current) {
            AD9834_StartRampUp(active, frequency, radio->tune_ramp_time_ms);
        } else if (frequency < current) {
            AD9834_StartRampDown(active, frequency, radio->tune_ramp_time_ms);
        }
    } else {
        AD9834_SetFrequency(active, frequency, AD9834_FREQ_REG_0);
    }

    // Update LO and BFO accordingly
    RadioOsc_UpdateLO(radio);
    RadioOsc_UpdateBFO(radio);
}

/**
 * @brief Tune up by step size
 */
void RadioOsc_TuneUp(RadioOscillator *radio) {
    if (!radio) return;
    RadioOsc_SetFrequency(radio, radio->frequency + radio->step_size);
}

/**
 * @brief Tune down by step size
 */
void RadioOsc_TuneDown(RadioOscillator *radio) {
    if (!radio) return;
    if (radio->frequency > radio->step_size) {
        RadioOsc_SetFrequency(radio, radio->frequency - radio->step_size);
    }
}

/**
 * @brief Set tuning step size
 */
void RadioOsc_SetStepSize(RadioOscillator *radio, uint32_t step_hz) {
    if (!radio) return;
    radio->step_size = step_hz;
}

/**
 * @brief Get current frequency
 */
uint32_t RadioOsc_GetFrequency(RadioOscillator *radio) {
    if (!radio) return 0;
    return radio->frequency;
}

/**
 * @brief Select VFO A or B
 */
void RadioOsc_SelectVFO(RadioOscillator *radio, VFO_Select vfo) {
    if (!radio) return;
    radio->active_vfo = vfo;

    // Read frequency from newly selected VFO
    AD9834_Instance *active = (vfo == VFO_A) ? radio->vfo_a : radio->vfo_b;
    radio->frequency = AD9834_GetCurrentFrequency(active);

    RadioOsc_UpdateLO(radio);
    RadioOsc_UpdateBFO(radio);
}

/**
 * @brief Swap VFO A and B
 */
void RadioOsc_SwapVFO(RadioOscillator *radio) {
    if (!radio) return;
    radio->active_vfo = (radio->active_vfo == VFO_A) ? VFO_B : VFO_A;
    RadioOsc_SelectVFO(radio, radio->active_vfo);
}

/**
 * @brief Copy active VFO to inactive
 */
void RadioOsc_CopyVFO(RadioOscillator *radio) {
    if (!radio) return;

    AD9834_Instance *src = (radio->active_vfo == VFO_A) ? radio->vfo_a : radio->vfo_b;
    AD9834_Instance *dst = (radio->active_vfo == VFO_A) ? radio->vfo_b : radio->vfo_a;

    uint32_t freq = AD9834_GetCurrentFrequency(src);
    AD9834_SetFrequency(dst, freq, AD9834_FREQ_REG_0);
}

/**
 * @brief Get active VFO
 */
VFO_Select RadioOsc_GetActiveVFO(RadioOscillator *radio) {
    if (!radio) return VFO_A;
    return radio->active_vfo;
}

/**
 * @brief Set operating mode
 */
void RadioOsc_SetMode(RadioOscillator *radio, RadioMode mode) {
    if (!radio) return;
    radio->mode = mode;
    RadioOsc_UpdateBFO(radio);
}

/**
 * @brief Get operating mode
 */
RadioMode RadioOsc_GetMode(RadioOscillator *radio) {
    if (!radio) return RADIO_MODE_USB;
    return radio->mode;
}

/**
 * @brief Set band
 */
void RadioOsc_SetBand(RadioOscillator *radio, RadioBand band) {
    if (!radio) return;

    radio->band = band;
    const BandInfo *info = RadioOsc_GetBandInfo(band);

    // Set to default frequency for this band
    RadioOsc_SetFrequency(radio, info->default_freq);
}

/**
 * @brief Get current band
 */
RadioBand RadioOsc_GetBand(RadioOscillator *radio) {
    if (!radio) return BAND_40M;
    return radio->band;
}

/**
 * @brief Get band information
 */
const BandInfo* RadioOsc_GetBandInfo(RadioBand band) {
    if (band >= 0 && band < sizeof(band_table)/sizeof(band_table[0])) {
        return &band_table[band];
    }
    return &band_table[BAND_40M];  // Default
}

/**
 * @brief Switch to TX mode
 */
void RadioOsc_SetTX(RadioOscillator *radio) {
    if (!radio) return;

    radio->state = RADIO_TX;

    // Determine TX frequency (split or normal)
    uint32_t tx_freq = radio->frequency;

    if (radio->split_mode) {
        // Use inactive VFO for TX
        AD9834_Instance *tx_vfo = (radio->active_vfo == VFO_A) ? radio->vfo_b : radio->vfo_a;
        tx_freq = AD9834_GetCurrentFrequency(tx_vfo);
    }

    // Apply XIT
    tx_freq += radio->xit_offset;

    // Update oscillators for TX
    AD9834_Instance *tx_vfo = (radio->active_vfo == VFO_A) ? radio->vfo_a : radio->vfo_b;
    if (radio->split_mode) {
        tx_vfo = (radio->active_vfo == VFO_A) ? radio->vfo_b : radio->vfo_a;
    }

    // Enable TX VFO with ramp
    AD9834_Start(tx_vfo, radio->tx_ramp_time_ms);

    RadioOsc_UpdateLO(radio);
}

/**
 * @brief Switch to RX mode
 */
void RadioOsc_SetRX(RadioOscillator *radio) {
    if (!radio) return;

    radio->state = RADIO_RX;

    // Stop TX VFO with ramp
    AD9834_Instance *tx_vfo = (radio->split_mode) ?
        ((radio->active_vfo == VFO_A) ? radio->vfo_b : radio->vfo_a) :
        ((radio->active_vfo == VFO_A) ? radio->vfo_a : radio->vfo_b);

    AD9834_Stop(tx_vfo, radio->tx_ramp_time_ms);

    // Enable RX VFO
    AD9834_Instance *rx_vfo = (radio->active_vfo == VFO_A) ? radio->vfo_a : radio->vfo_b;
    AD9834_EnableOutput(rx_vfo, true);

    // Apply RIT
    uint32_t rx_freq = radio->frequency + radio->rit_offset;
    AD9834_SetFrequency(rx_vfo, rx_freq, AD9834_FREQ_REG_0);

    RadioOsc_UpdateLO(radio);
    RadioOsc_UpdateBFO(radio);
}

/**
 * @brief Get TX/RX state
 */
RadioState RadioOsc_GetState(RadioOscillator *radio) {
    if (!radio) return RADIO_RX;
    return radio->state;
}

/**
 * @brief Set RIT offset
 */
void RadioOsc_SetRIT(RadioOscillator *radio, int32_t offset_hz) {
    if (!radio) return;
    radio->rit_offset = offset_hz;
    if (radio->state == RADIO_RX) {
        RadioOsc_SetRX(radio);  // Update with new offset
    }
}

/**
 * @brief Set XIT offset
 */
void RadioOsc_SetXIT(RadioOscillator *radio, int32_t offset_hz) {
    if (!radio) return;
    radio->xit_offset = offset_hz;
}

/**
 * @brief Clear RIT
 */
void RadioOsc_ClearRIT(RadioOscillator *radio) {
    RadioOsc_SetRIT(radio, 0);
}

/**
 * @brief Clear XIT
 */
void RadioOsc_ClearXIT(RadioOscillator *radio) {
    RadioOsc_SetXIT(radio, 0);
}

/**
 * @brief Enable/disable split operation
 */
void RadioOsc_EnableSplit(RadioOscillator *radio, bool enable) {
    if (!radio) return;
    radio->split_mode = enable;
}

/**
 * @brief Check if split is enabled
 */
bool RadioOsc_IsSplitEnabled(RadioOscillator *radio) {
    if (!radio) return false;
    return radio->split_mode;
}

/**
 * @brief CW key down
 */
void RadioOsc_KeyDown(RadioOscillator *radio) {
    if (!radio) return;
    RadioOsc_SetTX(radio);
}

/**
 * @brief CW key up
 */
void RadioOsc_KeyUp(RadioOscillator *radio) {
    if (!radio) return;
    RadioOsc_SetRX(radio);
}

/**
 * @brief Send FSK modulation (for RTTY, FT8, etc.)
 */
void RadioOsc_SendFSK(RadioOscillator *radio, bool mark) {
    if (!radio) return;

    AD9834_Instance *tx_vfo = (radio->active_vfo == VFO_A) ? radio->vfo_a : radio->vfo_b;

    // Use dual frequency registers for FSK
    if (mark) {
        AD9834_SelectFreqReg(tx_vfo, AD9834_FREQ_REG_0);  // Mark frequency
    } else {
        AD9834_SelectFreqReg(tx_vfo, AD9834_FREQ_REG_1);  // Space frequency
    }
}

/**
 * @brief Set CW offset frequency
 */
void RadioOsc_SetCWOffset(RadioOscillator *radio, uint32_t offset_hz) {
    if (!radio) return;
    radio->bfo_cw_offset = offset_hz;
    if (radio->mode == RADIO_MODE_CW) {
        RadioOsc_UpdateBFO(radio);
    }
}

/**
 * @brief Set frequency correction for calibration
 */
void RadioOsc_SetFrequencyCorrection(RadioOscillator *radio, int32_t correction_hz) {
    if (!radio) return;
    radio->frequency_correction = correction_hz;
}

/**
 * @brief Update loop - call periodically for ramping
 */
void RadioOsc_Update(RadioOscillator *radio) {
    if (!radio) return;

    // Update all ramps
    AD9834_UpdateRamp(radio->vfo_a);
    AD9834_UpdateRamp(radio->vfo_b);
    AD9834_UpdateRamp(radio->lo);
    AD9834_UpdateRamp(radio->bfo);
}

/**
 * @brief Calculate LO frequency based on RF frequency and IF
 */
static uint32_t RadioOsc_CalculateLOFreq(RadioOscillator *radio, uint32_t rf_freq) {
    // For superheterodyne: LO = RF + IF (high side injection)
    // or LO = RF - IF (low side injection)
    // Using high-side injection here
    return rf_freq + radio->if_frequency;
}

/**
 * @brief Update Local Oscillator based on current frequency
 */
static void RadioOsc_UpdateLO(RadioOscillator *radio) {
    if (!radio || !radio->lo) return;

    uint32_t lo_freq = RadioOsc_CalculateLOFreq(radio, radio->frequency);
    AD9834_SetFrequency(radio->lo, lo_freq, AD9834_FREQ_REG_0);
    AD9834_EnableOutput(radio->lo, true);
}

/**
 * @brief Update BFO based on current mode
 */
static void RadioOsc_UpdateBFO(RadioOscillator *radio) {
    if (!radio || !radio->bfo) return;

    uint32_t bfo_freq = radio->if_frequency;

    switch (radio->mode) {
        case RADIO_MODE_USB:
            bfo_freq += radio->bfo_usb_offset;
            break;

        case RADIO_MODE_LSB:
            bfo_freq += radio->bfo_lsb_offset;
            break;

        case RADIO_MODE_CW:
            bfo_freq += radio->bfo_cw_offset;
            break;

        case RADIO_MODE_AM:
            // AM uses IF frequency directly
            break;

        case RADIO_MODE_FM:
        case RADIO_MODE_DATA:
            bfo_freq += radio->bfo_usb_offset;  // Default to USB offset
            break;
    }

    AD9834_SetFrequency(radio->bfo, bfo_freq, AD9834_FREQ_REG_0);
    AD9834_EnableOutput(radio->bfo, true);
}
