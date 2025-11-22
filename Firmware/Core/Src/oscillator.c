/*
 * oscillator.c - AD9834 DDS Signal Generator Driver Implementation
 *
 * Class-based implementation supporting multiple AD9834 instances
 * with amplitude control and frequency ramping capabilities
 */

#include "oscillator.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Static instance pool for memory management */
static AD9834_Instance instance_pool[AD9834_MAX_INSTANCES];
static bool instance_allocated[AD9834_MAX_INSTANCES] = {false};

/* Private helper functions */
static void AD9834_WriteFreqReg(AD9834_Instance *inst, uint32_t freq_reg, AD9834_FreqReg reg);
static void AD9834_WritePhaseReg(AD9834_Instance *inst, uint16_t phase_reg, AD9834_PhaseReg reg);
static void AD9834_UpdateControlReg(AD9834_Instance *inst);

/**
 * @brief Create a new AD9834 instance
 */
AD9834_Instance* AD9834_Create(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port,
                               uint16_t cs_pin, uint32_t mclk) {
    if (!hspi || !cs_port) {
        return NULL;
    }

    // Find an available instance slot
    for (int i = 0; i < AD9834_MAX_INSTANCES; i++) {
        if (!instance_allocated[i]) {
            instance_allocated[i] = true;
            AD9834_Instance *inst = &instance_pool[i];

            // Clear the instance
            memset(inst, 0, sizeof(AD9834_Instance));

            // Set hardware configuration
            inst->hspi = hspi;
            inst->cs_port = cs_port;
            inst->cs_pin = cs_pin;
            inst->mclk_freq = (mclk > 0) ? mclk : AD9834_MCLK;

            // Set default values
            inst->amplitude = 1.0f;
            inst->target_amplitude = 1.0f;
            inst->active_freq_reg = AD9834_FREQ_REG_0;
            inst->active_phase_reg = AD9834_PHASE_REG_0;
            inst->waveform = AD9834_WAVEFORM_SINE;
            inst->ramp_state = AD9834_RAMP_IDLE;

            return inst;
        }
    }

    return NULL;  // No available instances
}

/**
 * @brief Destroy an AD9834 instance
 */
void AD9834_Destroy(AD9834_Instance *inst) {
    if (!inst) return;

    // Find and free the instance
    for (int i = 0; i < AD9834_MAX_INSTANCES; i++) {
        if (&instance_pool[i] == inst) {
            instance_allocated[i] = false;
            memset(inst, 0, sizeof(AD9834_Instance));
            break;
        }
    }
}

/**
 * @brief Write 16-bit data to AD9834 via SPI
 */
void AD9834_WriteData(AD9834_Instance *inst, uint16_t data) {
    if (!inst || !inst->hspi) return;

    uint8_t buffer[2];
    buffer[0] = (data >> 8) & 0xFF;  // MSB first
    buffer[1] = data & 0xFF;          // LSB second

    // CS low
    HAL_GPIO_WritePin(inst->cs_port, inst->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(inst->hspi, buffer, 2, HAL_MAX_DELAY);
    // CS high
    HAL_GPIO_WritePin(inst->cs_port, inst->cs_pin, GPIO_PIN_SET);
}

/**
 * @brief Initialize AD9834 DDS module
 */
void AD9834_Init(AD9834_Instance *inst) {
    if (!inst) return;

    // Set CS high initially
    HAL_GPIO_WritePin(inst->cs_port, inst->cs_pin, GPIO_PIN_SET);
    HAL_Delay(10);

    // Reset the device
    inst->control_reg = AD9834_RESET;
    AD9834_WriteData(inst, inst->control_reg);
    HAL_Delay(10);

    // Configure for 28-bit frequency writes, sine wave
    inst->control_reg = AD9834_B28;
    AD9834_WriteData(inst, inst->control_reg);

    // Set default frequency to 1 kHz on FREQ0
    AD9834_SetFrequency(inst, 1000, AD9834_FREQ_REG_0);

    // Set default phase to 0 degrees
    AD9834_SetPhase(inst, 0.0f, AD9834_PHASE_REG_0);
    AD9834_SetPhase(inst, 0.0f, AD9834_PHASE_REG_1);

    // Clear reset and enable output
    inst->control_reg &= ~AD9834_RESET;
    AD9834_WriteData(inst, inst->control_reg);

    inst->initialized = true;
    inst->output_enabled = true;
}

/**
 * @brief Software reset
 */
void AD9834_Reset(AD9834_Instance *inst) {
    if (!inst) return;

    inst->control_reg |= AD9834_RESET;
    AD9834_WriteData(inst, inst->control_reg);
    HAL_Delay(1);
    inst->control_reg &= ~AD9834_RESET;
    AD9834_WriteData(inst, inst->control_reg);
}

/**
 * @brief Write frequency register value to hardware
 */
static void AD9834_WriteFreqReg(AD9834_Instance *inst, uint32_t freq_reg, AD9834_FreqReg reg) {
    uint16_t reg_addr = (reg == AD9834_FREQ_REG_0) ? AD9834_REG_FREQ0 : AD9834_REG_FREQ1;

    // Write LSB (lower 14 bits)
    uint16_t lsb = (freq_reg & 0x3FFF) | reg_addr;
    AD9834_WriteData(inst, lsb);

    // Write MSB (upper 14 bits)
    uint16_t msb = ((freq_reg >> 14) & 0x3FFF) | reg_addr;
    AD9834_WriteData(inst, msb);
}

/**
 * @brief Set output frequency (integer Hz)
 */
void AD9834_SetFrequency(AD9834_Instance *inst, uint32_t frequency, AD9834_FreqReg reg) {
    if (!inst) return;

    // Calculate frequency register value
    // FREQ_REG = (Desired Frequency × 2^28) / MCLK
    uint32_t freq_reg = (uint32_t)(((uint64_t)frequency * (1ULL << 28)) / inst->mclk_freq);

    // Store current frequency
    inst->current_freq[reg] = frequency;

    // Write to hardware
    AD9834_WriteFreqReg(inst, freq_reg, reg);
}

/**
 * @brief Set output frequency (float Hz)
 */
void AD9834_SetFrequencyFloat(AD9834_Instance *inst, float frequency, AD9834_FreqReg reg) {
    if (!inst) return;

    uint32_t freq_reg = (uint32_t)((frequency * 268435456.0f) / (float)inst->mclk_freq);

    // Store current frequency
    inst->current_freq[reg] = (uint32_t)frequency;

    // Write to hardware
    AD9834_WriteFreqReg(inst, freq_reg, reg);
}

/**
 * @brief Select active frequency register
 */
void AD9834_SelectFreqReg(AD9834_Instance *inst, AD9834_FreqReg reg) {
    if (!inst) return;

    inst->active_freq_reg = reg;

    if (reg == AD9834_FREQ_REG_1) {
        inst->control_reg |= AD9834_FSELECT;
    } else {
        inst->control_reg &= ~AD9834_FSELECT;
    }

    AD9834_UpdateControlReg(inst);
}

/**
 * @brief Get currently active frequency register
 */
AD9834_FreqReg AD9834_GetActiveFreqReg(AD9834_Instance *inst) {
    if (!inst) return AD9834_FREQ_REG_0;
    return inst->active_freq_reg;
}

/**
 * @brief Write phase register value to hardware
 */
static void AD9834_WritePhaseReg(AD9834_Instance *inst, uint16_t phase_reg, AD9834_PhaseReg reg) {
    uint16_t reg_addr = (reg == AD9834_PHASE_REG_0) ? AD9834_REG_PHASE0 : AD9834_REG_PHASE1;
    uint16_t data = (phase_reg & 0x0FFF) | reg_addr;
    AD9834_WriteData(inst, data);
}

/**
 * @brief Set phase in degrees (0 to 360)
 */
void AD9834_SetPhase(AD9834_Instance *inst, float phase_deg, AD9834_PhaseReg reg) {
    if (!inst) return;

    // Normalize phase to 0-360
    while (phase_deg < 0) phase_deg += 360.0f;
    while (phase_deg >= 360.0f) phase_deg -= 360.0f;

    // Convert to 12-bit register value (0-4095)
    uint16_t phase_reg = (uint16_t)((phase_deg / 360.0f) * 4096.0f);

    // Store current phase
    inst->current_phase[reg] = phase_reg;

    // Write to hardware
    AD9834_WritePhaseReg(inst, phase_reg, reg);
}

/**
 * @brief Select active phase register
 */
void AD9834_SelectPhaseReg(AD9834_Instance *inst, AD9834_PhaseReg reg) {
    if (!inst) return;

    inst->active_phase_reg = reg;

    if (reg == AD9834_PHASE_REG_1) {
        inst->control_reg |= AD9834_PSELECT;
    } else {
        inst->control_reg &= ~AD9834_PSELECT;
    }

    AD9834_UpdateControlReg(inst);
}

/**
 * @brief Set waveform type
 */
void AD9834_SetWaveform(AD9834_Instance *inst, AD9834_WaveformType waveform) {
    if (!inst) return;

    inst->waveform = waveform;

    switch (waveform) {
        case AD9834_WAVEFORM_SINE:
            inst->control_reg &= ~(AD9834_MODE | AD9834_OPBITEN | AD9834_DIV2);
            break;

        case AD9834_WAVEFORM_TRIANGLE:
            inst->control_reg |= AD9834_MODE;
            inst->control_reg &= ~(AD9834_OPBITEN | AD9834_DIV2);
            break;

        case AD9834_WAVEFORM_SQUARE:
            inst->control_reg |= (AD9834_OPBITEN | AD9834_DIV2);
            inst->control_reg &= ~AD9834_MODE;
            break;
    }

    AD9834_UpdateControlReg(inst);
}

/**
 * @brief Set amplitude (0.0 to 1.0)
 * Note: AD9834 doesn't have built-in amplitude control.
 * This value is stored for external DAC/VGA control
 */
void AD9834_SetAmplitude(AD9834_Instance *inst, float amplitude) {
    if (!inst) return;

    // Clamp to valid range
    if (amplitude < 0.0f) amplitude = 0.0f;
    if (amplitude > 1.0f) amplitude = 1.0f;

    inst->target_amplitude = amplitude;
    inst->amplitude = amplitude;

    // TODO: Interface with external DAC or VGA here
    // For example, if using an I2C DAC:
    // DAC_SetOutput(inst->amplitude);
}

/**
 * @brief Get current amplitude
 */
float AD9834_GetAmplitude(AD9834_Instance *inst) {
    if (!inst) return 0.0f;
    return inst->amplitude;
}

/**
 * @brief Enable or disable output
 */
void AD9834_EnableOutput(AD9834_Instance *inst, bool enable) {
    if (!inst) return;

    inst->output_enabled = enable;

    if (enable) {
        inst->control_reg &= ~(AD9834_SLEEP1 | AD9834_SLEEP12);
    } else {
        inst->control_reg |= (AD9834_SLEEP1 | AD9834_SLEEP12);
    }

    AD9834_UpdateControlReg(inst);
}

/**
 * @brief Check if output is enabled
 */
bool AD9834_IsOutputEnabled(AD9834_Instance *inst) {
    if (!inst) return false;
    return inst->output_enabled;
}

/**
 * @brief Update control register to hardware
 */
static void AD9834_UpdateControlReg(AD9834_Instance *inst) {
    if (!inst) return;
    AD9834_WriteData(inst, inst->control_reg);
}

/**
 * @brief Start frequency ramp up
 */
void AD9834_StartRampUp(AD9834_Instance *inst, uint32_t target_freq, uint32_t duration_ms) {
    if (!inst) return;

    inst->ramp_state = AD9834_RAMP_UP;
    inst->ramp_start_freq = inst->current_freq[inst->active_freq_reg];
    inst->ramp_target_freq = target_freq;
    inst->ramp_duration_ms = duration_ms;
    inst->ramp_start_time = HAL_GetTick();

    // Calculate step count (update every 10ms for smooth ramping)
    inst->ramp_step_count = duration_ms / 10;
    if (inst->ramp_step_count == 0) inst->ramp_step_count = 1;
    inst->ramp_current_step = 0;
}

/**
 * @brief Start frequency ramp down
 */
void AD9834_StartRampDown(AD9834_Instance *inst, uint32_t target_freq, uint32_t duration_ms) {
    if (!inst) return;

    inst->ramp_state = AD9834_RAMP_DOWN;
    inst->ramp_start_freq = inst->current_freq[inst->active_freq_reg];
    inst->ramp_target_freq = target_freq;
    inst->ramp_duration_ms = duration_ms;
    inst->ramp_start_time = HAL_GetTick();

    // Calculate step count
    inst->ramp_step_count = duration_ms / 10;
    if (inst->ramp_step_count == 0) inst->ramp_step_count = 1;
    inst->ramp_current_step = 0;
}

/**
 * @brief Stop any active ramping
 */
void AD9834_StopRamp(AD9834_Instance *inst) {
    if (!inst) return;
    inst->ramp_state = AD9834_RAMP_IDLE;
}

/**
 * @brief Update frequency ramping (call periodically in main loop or timer)
 */
void AD9834_UpdateRamp(AD9834_Instance *inst) {
    if (!inst || inst->ramp_state == AD9834_RAMP_IDLE) return;

    uint32_t elapsed = HAL_GetTick() - inst->ramp_start_time;

    // Check if ramp is complete
    if (elapsed >= inst->ramp_duration_ms) {
        AD9834_SetFrequency(inst, inst->ramp_target_freq, inst->active_freq_reg);
        inst->ramp_state = AD9834_RAMP_IDLE;
        return;
    }

    // Calculate current frequency using linear interpolation
    float progress = (float)elapsed / (float)inst->ramp_duration_ms;

    int32_t freq_diff = (int32_t)inst->ramp_target_freq - (int32_t)inst->ramp_start_freq;
    uint32_t current_freq = inst->ramp_start_freq + (uint32_t)((float)freq_diff * progress);

    // Update frequency
    AD9834_SetFrequency(inst, current_freq, inst->active_freq_reg);
}

/**
 * @brief Check if ramping is active
 */
bool AD9834_IsRamping(AD9834_Instance *inst) {
    if (!inst) return false;
    return (inst->ramp_state != AD9834_RAMP_IDLE);
}

/**
 * @brief Start output with optional frequency ramp
 */
void AD9834_Start(AD9834_Instance *inst, uint32_t ramp_ms) {
    if (!inst) return;

    if (ramp_ms > 0) {
        // Start from DC (0 Hz) and ramp to current frequency
        uint32_t target_freq = inst->current_freq[inst->active_freq_reg];
        AD9834_SetFrequency(inst, 0, inst->active_freq_reg);
        AD9834_EnableOutput(inst, true);
        AD9834_StartRampUp(inst, target_freq, ramp_ms);
    } else {
        // Immediate start
        AD9834_EnableOutput(inst, true);
    }
}

/**
 * @brief Stop output with optional frequency ramp
 */
void AD9834_Stop(AD9834_Instance *inst, uint32_t ramp_ms) {
    if (!inst) return;

    if (ramp_ms > 0) {
        // Ramp down to DC (0 Hz) then disable
        AD9834_StartRampDown(inst, 0, ramp_ms);
        // Note: User should call AD9834_EnableOutput(inst, false) after ramp completes
    } else {
        // Immediate stop
        AD9834_EnableOutput(inst, false);
    }
}

/**
 * @brief Get current frequency of active register
 */
uint32_t AD9834_GetCurrentFrequency(AD9834_Instance *inst) {
    if (!inst) return 0;
    return inst->current_freq[inst->active_freq_reg];
}

/**
 * @brief Put DAC to sleep (power saving mode)
 */
void AD9834_SleepDAC(AD9834_Instance *inst, bool sleep) {
    if (!inst) return;

    if (sleep) {
        inst->control_reg |= AD9834_SLEEP12;
    } else {
        inst->control_reg &= ~AD9834_SLEEP12;
    }

    AD9834_UpdateControlReg(inst);
}
