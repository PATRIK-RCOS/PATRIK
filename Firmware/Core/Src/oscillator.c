/*
 * oscillator.c - AD9834 DDS Signal Generator Driver Implementation
 */

#include "oscillator.h"

extern SPI_HandleTypeDef hspi1;

static uint32_t AD9834_MCLK_Freq = AD9834_MCLK;
static uint16_t control_reg = 0;

/**
 * @brief Write 16-bit data to AD9834 via SPI
 */
void AD9834_WriteData(uint16_t data) {
    uint8_t buffer[2];
    buffer[0] = (data >> 8) & 0xFF;  // MSB first
    buffer[1] = data & 0xFF;          // LSB second
    
    AD9834_CS_LOW();
    HAL_SPI_Transmit(&hspi1, buffer, 2, HAL_MAX_DELAY);
    AD9834_CS_HIGH();
}

/**
 * @brief Initialize AD9834 DDS module
 */
void AD9834_Init(uint32_t mclk) {
    if (mclk > 0) {
        AD9834_MCLK_Freq = mclk;
    }
    
    // Set CS high initially
    AD9834_CS_HIGH();
    HAL_Delay(10);
    
    // Reset the device
    control_reg = AD9834_RESET;
    AD9834_WriteData(control_reg);
    HAL_Delay(10);
    
    // Configure for 28-bit frequency writes, sine wave
    control_reg = AD9834_B28;
    AD9834_WriteData(control_reg);
    
    // Set default frequency to 1 kHz
    AD9834_SetFrequency(1000);
    
    // Clear reset and enable output
    control_reg &= ~AD9834_RESET;
    AD9834_WriteData(control_reg);
}

/**
 * @brief Set output frequency (integer Hz)
 */
void AD9834_SetFrequency(uint32_t frequency) {
    // Calculate frequency register value
    // FREQ_REG = (Desired Frequency × 2^28) / MCLK
    uint32_t freq_reg = (uint32_t)(((uint64_t)frequency * (1ULL << 28)) / AD9834_MCLK_Freq);
    
    // Write LSB (lower 14 bits)
    uint16_t lsb = (freq_reg & 0x3FFF) | AD9834_REG_FREQ0;
    AD9834_WriteData(lsb);
    
    // Write MSB (upper 14 bits)
    uint16_t msb = ((freq_reg >> 14) & 0x3FFF) | AD9834_REG_FREQ0;
    AD9834_WriteData(msb);
}

/**
 * @brief Set output frequency (float Hz)
 */
void AD9834_SetFrequencyFloat(float frequency) {
    uint32_t freq_reg = (uint32_t)((frequency * 268435456.0f) / (float)AD9834_MCLK_Freq);
    
    uint16_t lsb = (freq_reg & 0x3FFF) | AD9834_REG_FREQ0;
    AD9834_WriteData(lsb);
    
    uint16_t msb = ((freq_reg >> 14) & 0x3FFF) | AD9834_REG_FREQ0;
    AD9834_WriteData(msb);
}

/**
 * @brief Set waveform type
 */
void AD9834_SetWaveform(AD9834_WaveformType waveform) {
    switch (waveform) {
        case AD9834_WAVEFORM_SINE:
            control_reg &= ~(AD9834_MODE | AD9834_OPBITEN | AD9834_DIV2);
            break;
            
        case AD9834_WAVEFORM_TRIANGLE:
            control_reg |= AD9834_MODE;
            control_reg &= ~(AD9834_OPBITEN | AD9834_DIV2);
            break;
            
        case AD9834_WAVEFORM_SQUARE:
            control_reg |= (AD9834_OPBITEN | AD9834_DIV2);
            control_reg &= ~AD9834_MODE;
            break;
    }
    
    AD9834_WriteData(control_reg);
}

/**
 * @brief Enable or disable output
 */
void AD9834_EnableOutput(bool enable) {
    if (enable) {
        control_reg &= ~(AD9834_SLEEP1 | AD9834_SLEEP12);
    } else {
        control_reg |= (AD9834_SLEEP1 | AD9834_SLEEP12);
    }
    AD9834_WriteData(control_reg);
}

/**
 * @brief Software reset
 */
void AD9834_Reset(void) {
    control_reg |= AD9834_RESET;
    AD9834_WriteData(control_reg);
    HAL_Delay(1);
    control_reg &= ~AD9834_RESET;
    AD9834_WriteData(control_reg);
}
