/*
 * oscillator_example.c - High-level Radio Oscillator Usage Examples
 *
 * This demonstrates the high-level radio oscillator API for building
 * a complete HF transceiver system
 */

#include "radio_oscillator.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

/* Global radio instance */
static RadioOscillator *radio;

/**
 * @brief Initialize the radio system
 */
void Radio_SystemInit(void) {
    // Create radio oscillator system with 4 DDS chips
    // VFO A: PA0, VFO B: PA1, LO: PA2, BFO: PA3
    radio = RadioOsc_Create(&hspi1,
                           GPIOA, GPIO_PIN_0,  // VFO A
                           GPIOA, GPIO_PIN_1,  // VFO B
                           GPIOA, GPIO_PIN_2,  // LO
                           GPIOA, GPIO_PIN_3); // BFO

    // Initialize with 9 MHz IF
    RadioOsc_Init(radio, 9000000);

    // Set initial mode and band
    RadioOsc_SetMode(radio, RADIO_MODE_USB);
    RadioOsc_SetBand(radio, BAND_40M);

    // Configure tuning parameters
    RadioOsc_SetStepSize(radio, 100);  // 100 Hz steps
}

/**
 * @brief Example: Basic VFO tuning
 */
void Example_BasicTuning(void) {
    // Tune to FT8 frequency on 40m
    RadioOsc_SetFrequency(radio, 7074000);

    // Tune up/down with encoder
    RadioOsc_TuneUp(radio);    // +100 Hz
    RadioOsc_TuneDown(radio);  // -100 Hz

    // Change step size for faster tuning
    RadioOsc_SetStepSize(radio, 1000);  // 1 kHz steps
    RadioOsc_TuneUp(radio);  // +1 kHz
}

/**
 * @brief Example: Band changing
 */
void Example_BandChange(void) {
    // Switch to 20m band (automatically goes to default freq)
    RadioOsc_SetBand(radio, BAND_20M);

    // Get band information
    const BandInfo *info = RadioOsc_GetBandInfo(BAND_20M);
    // info->name = "20m"
    // info->default_freq = 14074000

    // Switch to 40m
    RadioOsc_SetBand(radio, BAND_40M);
}

/**
 * @brief Example: Mode switching
 */
void Example_ModeChange(void) {
    // Switch to LSB (for voice on lower HF bands)
    RadioOsc_SetMode(radio, RADIO_MODE_LSB);

    // Switch to CW for Morse code
    RadioOsc_SetMode(radio, RADIO_MODE_CW);

    // Switch to DATA mode for digital
    RadioOsc_SetMode(radio, RADIO_MODE_DATA);
}

/**
 * @brief Example: VFO A/B operation
 */
void Example_VFO_AB(void) {
    // Set VFO A to calling frequency
    RadioOsc_SelectVFO(radio, VFO_A);
    RadioOsc_SetFrequency(radio, 7074000);

    // Set VFO B to a different frequency
    RadioOsc_SelectVFO(radio, VFO_B);
    RadioOsc_SetFrequency(radio, 7076000);

    // Quick toggle between frequencies
    RadioOsc_SelectVFO(radio, VFO_A);
    HAL_Delay(1000);
    RadioOsc_SelectVFO(radio, VFO_B);

    // Swap VFO A and B
    RadioOsc_SwapVFO(radio);

    // Copy VFO A to B (or vice versa)
    RadioOsc_CopyVFO(radio);
}

/**
 * @brief Example: Split operation (TX on one freq, RX on another)
 */
void Example_SplitOperation(void) {
    // Set up split operation for DX pile-up
    RadioOsc_SelectVFO(radio, VFO_A);
    RadioOsc_SetFrequency(radio, 14250000);  // Listen on VFO A

    RadioOsc_SelectVFO(radio, VFO_B);
    RadioOsc_SetFrequency(radio, 14260000);  // Transmit on VFO B

    RadioOsc_SelectVFO(radio, VFO_A);  // Listen on A
    RadioOsc_EnableSplit(radio, true);

    // Now when PTT is pressed, radio will TX on VFO B
    // but RX on VFO A
}

/**
 * @brief Example: RIT/XIT (Incremental Tuning)
 */
void Example_RIT_XIT(void) {
    // Main frequency
    RadioOsc_SetFrequency(radio, 14250000);

    // Station is slightly off frequency
    // Use RIT to adjust RX without changing TX
    RadioOsc_SetRIT(radio, 150);  // +150 Hz on RX only

    // Or use XIT for TX offset
    RadioOsc_SetXIT(radio, -100);  // -100 Hz on TX only

    // Clear when done
    RadioOsc_ClearRIT(radio);
    RadioOsc_ClearXIT(radio);
}

/**
 * @brief Example: CW (Morse Code) transmission
 */
void Example_CW_Operation(void) {
    // Set CW mode with 700 Hz sidetone
    RadioOsc_SetMode(radio, RADIO_MODE_CW);
    RadioOsc_SetCWOffset(radio, 700);
    RadioOsc_SetFrequency(radio, 7030000);  // CW portion of 40m

    // Simulate sending "CQ"
    // C: -.-.
    RadioOsc_KeyDown(radio);  // dah
    HAL_Delay(150);
    RadioOsc_KeyUp(radio);
    HAL_Delay(50);

    RadioOsc_KeyDown(radio);  // dit
    HAL_Delay(50);
    RadioOsc_KeyUp(radio);
    HAL_Delay(50);

    RadioOsc_KeyDown(radio);  // dah
    HAL_Delay(150);
    RadioOsc_KeyUp(radio);
    HAL_Delay(50);

    RadioOsc_KeyDown(radio);  // dit
    HAL_Delay(50);
    RadioOsc_KeyUp(radio);
    HAL_Delay(150);  // letter space

    // Q: --.-
    RadioOsc_KeyDown(radio);  // dah
    HAL_Delay(150);
    RadioOsc_KeyUp(radio);
    // ... etc
}

/**
 * @brief Example: FSK modulation for digital modes (RTTY, FT8, etc.)
 */
void Example_FSK_Modulation(void) {
    RadioOsc_SetMode(radio, RADIO_MODE_DATA);
    RadioOsc_SetFrequency(radio, 7074000);

    // Switch to TX
    RadioOsc_SetTX(radio);

    // Send binary data using FSK
    uint8_t data[] = {0xAA, 0x55};  // Alternating pattern

    for (int i = 0; i < sizeof(data); i++) {
        for (int bit = 0; bit < 8; bit++) {
            bool mark = (data[i] & (1 << bit)) != 0;
            RadioOsc_SendFSK(radio, mark);
            HAL_Delay(10);  // Baud rate dependent
        }
    }

    // Back to RX
    RadioOsc_SetRX(radio);
}

/**
 * @brief Example: SSB voice transmission
 */
void Example_SSB_Voice(void) {
    // Setup for voice on 40m
    RadioOsc_SetMode(radio, RADIO_MODE_LSB);  // LSB for 40m and below
    RadioOsc_SetFrequency(radio, 7200000);    // Voice portion

    // PTT pressed - go to TX
    RadioOsc_SetTX(radio);  // Smooth ramp up, no clicks

    // Audio is modulated by external mixer using BFO
    // Speak into microphone...
    HAL_Delay(5000);

    // PTT released - back to RX
    RadioOsc_SetRX(radio);  // Smooth ramp down
}

/**
 * @brief Example: Frequency calibration
 */
void Example_FrequencyCalibration(void) {
    // Tune to WWV (5, 10, 15, 20 MHz) or use GPS frequency reference
    RadioOsc_SetFrequency(radio, 10000000);  // WWV 10 MHz

    // Measure frequency error with counter
    // Let's say we're 50 Hz high

    int32_t correction = -50;  // Apply -50 Hz correction
    RadioOsc_SetFrequencyCorrection(radio, correction);

    // Now all frequencies will be corrected
}

/**
 * @brief Example: Complete QSO (contact)
 */
void Example_CompleteQSO(void) {
    // Listen for CQ on 40m FT8
    RadioOsc_SetMode(radio, RADIO_MODE_DATA);
    RadioOsc_SetBand(radio, BAND_40M);
    RadioOsc_SetFrequency(radio, 7074000);

    // Hear a station calling CQ on 7074.125 kHz
    RadioOsc_SetFrequency(radio, 7074125);

    // Reply on same frequency
    RadioOsc_SetTX(radio);

    // Send FT8 message via FSK
    // ... (FT8 encoding and transmission)

    RadioOsc_SetRX(radio);

    // Monitor for reply
    HAL_Delay(15000);  // FT8 period

    // Complete exchange, log contact
}

/**
 * @brief Main radio control loop
 */
void Radio_MainLoop(void) {
    Radio_SystemInit();

    while (1) {
        // Update all oscillator ramping
        RadioOsc_Update(radio);

        // Handle user input (encoder, buttons, etc.)
        // if (encoder_turned_right()) {
        //     RadioOsc_TuneUp(radio);
        // }
        // if (ptt_pressed()) {
        //     RadioOsc_SetTX(radio);
        // }

        // Update display with current frequency
        uint32_t freq = RadioOsc_GetFrequency(radio);
        RadioMode mode = RadioOsc_GetMode(radio);
        RadioBand band = RadioOsc_GetBand(radio);

        // Display_ShowFrequency(freq);
        // Display_ShowMode(mode);
        // Display_ShowBand(band);

        HAL_Delay(1);
    }
}

/**
 * @brief Example: Memory channels
 */
void Example_MemoryChannels(void) {
    // Store current settings to VFO B as a "memory"
    RadioOsc_SelectVFO(radio, VFO_A);
    RadioOsc_SetFrequency(radio, 7074000);
    RadioOsc_SetMode(radio, RADIO_MODE_DATA);

    // Save to "memory" (VFO B)
    RadioOsc_CopyVFO(radio);

    // Tune around on VFO A
    RadioOsc_SetFrequency(radio, 7200000);
    RadioOsc_SetMode(radio, RADIO_MODE_LSB);

    // Recall memory
    RadioOsc_SelectVFO(radio, VFO_B);  // Back to 7074 kHz DATA mode
}

/**
 * @brief Example: Contest operation (fast frequency changes)
 */
void Example_ContestOperation(void) {
    // Set fast tuning with no ramp for contest operation
    radio->tune_ramp_time_ms = 0;  // Direct access for speed

    // Quick band changes
    RadioOsc_SetBand(radio, BAND_20M);
    RadioOsc_SetBand(radio, BAND_40M);
    RadioOsc_SetBand(radio, BAND_15M);

    // Scan for stations
    for (uint32_t freq = 7000000; freq < 7100000; freq += 3000) {
        RadioOsc_SetFrequency(radio, freq);
        HAL_Delay(100);  // Quick scan
        // Check S-meter for signals
    }
}
