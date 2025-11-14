/*
 * oscillator.h - AD9834 DDS Signal Generator Driver
 */

#ifndef INC_OSCILLATOR_H_
#define INC_OSCILLATOR_H_

#include "stm32h7xx_hal.h"
#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/* AD9834 Pin Definitions - Configure PA0 as GPIO Output in CubeMX */
#define AD9834_CS_Pin GPIO_PIN_0
#define AD9834_CS_GPIO_Port GPIOA

/* Control Macros */
#define AD9834_CS_LOW()  HAL_GPIO_WritePin(AD9834_CS_GPIO_Port, AD9834_CS_Pin, GPIO_PIN_RESET)
#define AD9834_CS_HIGH() HAL_GPIO_WritePin(AD9834_CS_GPIO_Port, AD9834_CS_Pin, GPIO_PIN_SET)

/* Register Addresses */
#define AD9834_REG_FREQ0    0x4000
#define AD9834_REG_FREQ1    0x8000
#define AD9834_REG_PHASE0   0xC000

/* Control Register Bits */
#define AD9834_B28          0x2000
#define AD9834_FSELECT      0x0800
#define AD9834_RESET        0x0100
#define AD9834_SLEEP1       0x0080
#define AD9834_SLEEP12      0x0040
#define AD9834_OPBITEN      0x0020
#define AD9834_DIV2         0x0008
#define AD9834_MODE         0x0002

/* Constants */
#define AD9834_MCLK         75000000UL

/* Waveform Types */
typedef enum {
    AD9834_WAVEFORM_SINE = 0,
    AD9834_WAVEFORM_TRIANGLE = 1,
    AD9834_WAVEFORM_SQUARE = 2
} AD9834_WaveformType;

/* Function Prototypes */
void AD9834_Init(uint32_t mclk);
void AD9834_WriteData(uint16_t data);
void AD9834_SetFrequency(uint32_t frequency);
void AD9834_SetFrequencyFloat(float frequency);
void AD9834_SetWaveform(AD9834_WaveformType waveform);
void AD9834_EnableOutput(bool enable);
void AD9834_Reset(void);

#endif /* INC_OSCILLATOR_H_ */
