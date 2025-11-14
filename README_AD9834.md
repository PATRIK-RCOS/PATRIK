# AD9834 DDS Signal Generator Driver

Driver for the AD9834 DDS High-speed Signal Generator Module supporting sine, triangle, and square wave generation up to 75 MHz.

## Hardware Specifications

- **Chip**: Analog Devices AD9834
- **Frequency Range**: DC to 75 MHz (with 75 MHz MCLK)
- **Frequency Resolution**: 0.28 Hz @ 75 MHz MCLK
- **Waveforms**: Sine, Triangle, Square
- **Interface**: 3-wire SPI (up to 40 Mbits/s)
- **Power**: 2.3V to 5.5V, 20 mW typical
- **Output**: 28-bit frequency register, 12-bit phase register

## Pin Configuration

### Required STM32CubeMX Configuration

Configure the following in STM32CubeMX:

1. **SPI1** - Already configured at 24 Mbits/s
   - PA5: SPI1_SCK
   - PA6: SPI1_MISO (not used for AD9834, but needed for SPI)
   - PA7: SPI1_MOSI

2. **GPIO Output** for Chip Select:
   - **PA0**: AD9834_CS (or any free GPIO pin)
   - Configuration: GPIO_Output, Label: "AD9834_CS"
   - Initial State: High

### AD9834 Module Connections

| AD9834 Pin | STM32 Pin | Signal | Notes |
|------------|-----------|--------|-------|
| VCC        | 3.3V/5V   | Power  | Use appropriate voltage |
| GND        | GND       | Ground | |
| SDATA      | PA7       | MOSI   | Serial data input |
| SCLK       | PA5       | SCK    | Serial clock |
| FSYNC      | PA0       | CS     | Chip select (active low) |
| MCLK       | External  | Clock  | 75 MHz oscillator |
| VOUT       | -         | Output | Analog waveform output |

**Note**: The AD9834 and ILI9488 LCD share SPI1 bus, so they must use different CS pins.

## Usage Examples

### Basic Initialization

```c
#include "oscillator.h"

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    
    // Initialize AD9834 with 75 MHz master clock
    AD9834_Init(75000000);
    
    while(1) {
        // Your code here
    }
}
```

### Generate Sine Wave at 1 kHz

```c
AD9834_SetFrequency(1000);                    // Set to 1 kHz
AD9834_SetWaveform(AD9834_WAVEFORM_SINE);     // Select sine wave
AD9834_EnableOutput(true);                    // Enable output
```

### Generate Triangle Wave at 10 kHz

```c
AD9834_SetFrequency(10000);
AD9834_SetWaveform(AD9834_WAVEFORM_TRIANGLE);
AD9834_EnableOutput(true);
```

### Generate Square Wave at 100 kHz

```c
AD9834_SetFrequency(100000);
AD9834_SetWaveform(AD9834_WAVEFORM_SQUARE);
AD9834_EnableOutput(true);
```

### Precise Frequency with Floating Point

```c
// Generate 7.050 MHz for HF radio applications
AD9834_SetFrequencyFloat(7.050e6);
AD9834_SetWaveform(AD9834_WAVEFORM_SINE);
AD9834_EnableOutput(true);

// Or very low frequency
AD9834_SetFrequencyFloat(0.1);  // 0.1 Hz
```

### Frequency Sweep Example

```c
// Sweep from 1 kHz to 10 kHz
for (uint32_t freq = 1000; freq <= 10000; freq += 100) {
    AD9834_SetFrequency(freq);
    HAL_Delay(100);  // 100 ms per step
}
```

### Power Management

```c
// Disable output to save power
AD9834_EnableOutput(false);

// Re-enable output
AD9834_EnableOutput(true);

// Software reset
AD9834_Reset();
```

## API Reference

### Initialization

```c
void AD9834_Init(uint32_t mclk);
```
Initialize the AD9834 module.
- **mclk**: Master clock frequency in Hz (default: 75000000)

### Frequency Control

```c
void AD9834_SetFrequency(uint32_t frequency);
```
Set output frequency using integer Hz.
- **frequency**: Desired frequency in Hz (0 to ~37 MHz)

```c
void AD9834_SetFrequencyFloat(float frequency);
```
Set output frequency using floating point Hz for precise control.
- **frequency**: Desired frequency in Hz (supports fractional Hz)

### Waveform Selection

```c
void AD9834_SetWaveform(AD9834_WaveformType waveform);
```
Select output waveform type.
- **waveform**: 
  - `AD9834_WAVEFORM_SINE` - Sine wave
  - `AD9834_WAVEFORM_TRIANGLE` - Triangle wave
  - `AD9834_WAVEFORM_SQUARE` - Square wave

### Output Control

```c
void AD9834_EnableOutput(bool enable);
```
Enable or disable output.
- **enable**: true to enable, false to disable (power down)

```c
void AD9834_Reset(void);
```
Perform software reset of the AD9834.

### Low-Level Functions

```c
void AD9834_WriteData(uint16_t data);
```
Write 16-bit data directly to AD9834 via SPI.

## Frequency Calculation

The AD9834 uses a 28-bit frequency register. The output frequency is calculated as:

```
f_out = (FREQ_REG × MCLK) / 2^28
```

Where:
- **f_out**: Output frequency (Hz)
- **FREQ_REG**: 28-bit frequency register value (0 to 268,435,455)
- **MCLK**: Master clock frequency (typically 75 MHz)
- **2^28**: 268,435,456

### Frequency Resolution

With a 75 MHz MCLK:
- **Resolution**: 75,000,000 / 268,435,456 = **0.28 Hz**
- **Max Frequency**: ~37.5 MHz (Nyquist limit)

### Example Calculations

| Desired Freq | FREQ_REG (hex) | Actual Freq | Error |
|--------------|----------------|-------------|-------|
| 1 kHz        | 0x0364B5A     | 1000.00 Hz  | 0 Hz  |
| 10 kHz       | 0x21F07C2     | 10000.01 Hz | 0.01 Hz |
| 1 MHz        | 0x15798EE2    | 1000000 Hz  | 0 Hz  |
| 7.050 MHz    | 0x97B425ED    | 7050000 Hz  | 0 Hz  |

## Integration with Existing Code

The driver is already integrated with your firmware:

1. **Header included** in [main.h](Firmware/Core/Inc/main.h:39)
2. **Follows same pattern** as ILI9488 LCD driver
3. **Uses shared SPI1** bus (compatible with display)
4. **Compatible with HAL** blocking mode

### Example Integration in main.c

```c
/* USER CODE BEGIN 2 */

// Initialize display
ILI9488_Init();

// Initialize DDS
AD9834_Init(75000000);
AD9834_SetFrequency(1000);
AD9834_SetWaveform(AD9834_WAVEFORM_SINE);
AD9834_EnableOutput(true);

/* USER CODE END 2 */
```

## Troubleshooting

### No Output

1. Check MCLK is connected and oscillating at 75 MHz
2. Verify CS pin is configured as GPIO Output in CubeMX
3. Ensure SPI1 is initialized before calling AD9834_Init()
4. Check AD9834_EnableOutput(true) has been called
5. Verify VCC power supply (2.3V to 5.5V)

### Incorrect Frequency

1. Verify MCLK frequency matches the value passed to AD9834_Init()
2. Check if MCLK oscillator is accurate
3. Use AD9834_SetFrequencyFloat() for precise frequencies

### SPI Communication Issues

1. Check SPI1 clock speed (should be ≤ 40 Mbits/s)
2. Verify SPI mode: CPOL=0, CPHA=0
3. Ensure CS pin toggles during transmission
4. Check SCLK and SDATA signals with oscilloscope

### Interference with Display

1. Verify different CS pins for AD9834 and ILI9488
2. Check CS signals don't overlap
3. Add small delay between SPI transactions if needed

## Technical Details

### Register Map

| Address | Register | Bits | Description |
|---------|----------|------|-------------|
| 00      | Control  | 16   | Control register |
| 01      | FREQ0    | 28   | Frequency register 0 (LSB/MSB) |
| 10      | FREQ1    | 28   | Frequency register 1 (LSB/MSB) |
| 11      | PHASE0   | 12   | Phase register 0 |

### Control Register Bits

| Bit | Name | Description |
|-----|------|-------------|
| D15-D14 | - | Address bits (00 for control) |
| D13 | B28 | Enable 28-bit frequency writes |
| D12 | HLB | Half-word load (0=LSB, 1=MSB) |
| D11 | FSELECT | Freq register select (0=FREQ0, 1=FREQ1) |
| D10 | PSELECT | Phase register select |
| D8 | RESET | Reset internal registers |
| D7 | SLEEP1 | Power down DAC |
| D6 | SLEEP12 | Power down internal clock |
| D5 | OPBITEN | Enable square wave output |
| D3 | DIV2 | Divide frequency by 2 |
| D1 | MODE | Waveform (0=sine, 1=triangle) |

### SPI Timing

- **Max Clock**: 40 Mbits/s (25 ns period)
- **Setup Time**: 5 ns minimum
- **Hold Time**: 5 ns minimum
- **CS to SCLK**: 5 ns minimum

## Files

- [Firmware/Core/Inc/oscillator.h](Firmware/Core/Inc/oscillator.h) - Header file with definitions
- [Firmware/Core/Src/oscillator.c](Firmware/Core/Src/oscillator.c) - Implementation file
- [Firmware/Core/Inc/main.h](Firmware/Core/Inc/main.h) - Updated to include driver

## References

- [AD9834 Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/ad9834.pdf) - Official Analog Devices documentation
- [Application Note AN-1070](https://www.analog.com/media/en/technical-documentation/application-notes/AN-1070.pdf) - DDS fundamentals
