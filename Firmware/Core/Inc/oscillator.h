/*
 * oscillator.h - AD9834 DDS Signal Generator Driver
 *
 * Class-based implementation supporting multiple AD9834 instances
 * with amplitude control and frequency ramping capabilities
 */

#ifndef INC_OSCILLATOR_H_
#define INC_OSCILLATOR_H_

#include "stm32h7xx_hal.h"
#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/* Register Addresses */
#define AD9834_REG_FREQ0    0x4000
#define AD9834_REG_FREQ1    0x8000
#define AD9834_REG_PHASE0   0xC000
#define AD9834_REG_PHASE1   0xE000

/* Control Register Bits */
#define AD9834_B28          0x2000
#define AD9834_HLB          0x1000
#define AD9834_FSELECT      0x0800
#define AD9834_PSELECT      0x0400
#define AD9834_RESET        0x0100
#define AD9834_SLEEP1       0x0080
#define AD9834_SLEEP12      0x0040
#define AD9834_OPBITEN      0x0020
#define AD9834_DIV2         0x0008
#define AD9834_MODE         0x0002

/* Constants */
#define AD9834_MCLK         75000000UL
#define AD9834_MAX_INSTANCES 4

/* Waveform Types */
typedef enum {
    AD9834_WAVEFORM_SINE = 0,
    AD9834_WAVEFORM_TRIANGLE = 1,
    AD9834_WAVEFORM_SQUARE = 2
} AD9834_WaveformType;

/* Frequency Register Selection */
typedef enum {
    AD9834_FREQ_REG_0 = 0,
    AD9834_FREQ_REG_1 = 1
} AD9834_FreqReg;

/* Phase Register Selection */
typedef enum {
    AD9834_PHASE_REG_0 = 0,
    AD9834_PHASE_REG_1 = 1
} AD9834_PhaseReg;

/* Ramp State */
typedef enum {
    AD9834_RAMP_IDLE = 0,
    AD9834_RAMP_UP,
    AD9834_RAMP_DOWN
} AD9834_RampState;

/* AD9834 Instance Structure */
typedef struct {
    /* Hardware configuration */
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    uint32_t mclk_freq;

    /* Current state */
    uint16_t control_reg;
    uint32_t current_freq[2];      // FREQ0 and FREQ1
    uint16_t current_phase[2];     // PHASE0 and PHASE1
    AD9834_FreqReg active_freq_reg;
    AD9834_PhaseReg active_phase_reg;
    AD9834_WaveformType waveform;

    /* Amplitude control (0.0 to 1.0) */
    float amplitude;
    float target_amplitude;

    /* Ramping control */
    AD9834_RampState ramp_state;
    uint32_t ramp_start_freq;
    uint32_t ramp_target_freq;
    uint32_t ramp_duration_ms;
    uint32_t ramp_start_time;
    uint32_t ramp_step_count;
    uint32_t ramp_current_step;

    /* Status flags */
    bool initialized;
    bool output_enabled;

} AD9834_Instance;

/* Function Prototypes */

/* Instance Management */
AD9834_Instance* AD9834_Create(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port,
                               uint16_t cs_pin, uint32_t mclk);
void AD9834_Destroy(AD9834_Instance *inst);
void AD9834_Init(AD9834_Instance *inst);
void AD9834_Reset(AD9834_Instance *inst);

/* Low-level I/O */
void AD9834_WriteData(AD9834_Instance *inst, uint16_t data);

/* Frequency Control */
void AD9834_SetFrequency(AD9834_Instance *inst, uint32_t frequency, AD9834_FreqReg reg);
void AD9834_SetFrequencyFloat(AD9834_Instance *inst, float frequency, AD9834_FreqReg reg);
void AD9834_SelectFreqReg(AD9834_Instance *inst, AD9834_FreqReg reg);
AD9834_FreqReg AD9834_GetActiveFreqReg(AD9834_Instance *inst);

/* Phase Control */
void AD9834_SetPhase(AD9834_Instance *inst, float phase_deg, AD9834_PhaseReg reg);
void AD9834_SelectPhaseReg(AD9834_Instance *inst, AD9834_PhaseReg reg);

/* Waveform Control */
void AD9834_SetWaveform(AD9834_Instance *inst, AD9834_WaveformType waveform);

/* Amplitude Control (software-based, requires external DAC or VGA) */
void AD9834_SetAmplitude(AD9834_Instance *inst, float amplitude);
float AD9834_GetAmplitude(AD9834_Instance *inst);

/* Output Control */
void AD9834_EnableOutput(AD9834_Instance *inst, bool enable);
bool AD9834_IsOutputEnabled(AD9834_Instance *inst);

/* Ramping Functions */
void AD9834_StartRampUp(AD9834_Instance *inst, uint32_t target_freq, uint32_t duration_ms);
void AD9834_StartRampDown(AD9834_Instance *inst, uint32_t target_freq, uint32_t duration_ms);
void AD9834_StopRamp(AD9834_Instance *inst);
void AD9834_UpdateRamp(AD9834_Instance *inst);
bool AD9834_IsRamping(AD9834_Instance *inst);

/* Start/Stop with Ramping */
void AD9834_Start(AD9834_Instance *inst, uint32_t ramp_ms);
void AD9834_Stop(AD9834_Instance *inst, uint32_t ramp_ms);

/* Utility Functions */
uint32_t AD9834_GetCurrentFrequency(AD9834_Instance *inst);
void AD9834_SleepDAC(AD9834_Instance *inst, bool sleep);

#endif /* INC_OSCILLATOR_H_ */
