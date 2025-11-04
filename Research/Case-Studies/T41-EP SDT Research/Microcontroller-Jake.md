# Teensy 4.1 Microcontroller

## Microcontroller Specifications
* Microprocessor - ARM Cortex-M7
  * Clock speed: 600 MHz
* FPU with 32 bit and 64 bit support
* 42 GPIO Pins
  * 8 Serial ports (3: SPI, 3: I2C)
  * 18 Analog Inputs
* 8 Mb Flash memory
* 1 Mb RAM
* Spots for 2 extra RAM or flash chips if necessary (requires modding)
* 4 Kb EEPROM (emulated)
* Digital Audio:
  * 2 I2S/TDM ports
  * 1 S/PDIF port
* Ethernet support over 3 pairs (instead of the usual 4)
* SD Card on the board

## Microcontroller pinout
* Vin: 5v input
* 0: Unused
* 1: Unused
* 2: INPUT: Volume encoder clock pin 
* 3: INPUT: Volume encoder data pin
* 4: INPUT: Fine Tune encoder clock pin
* 5: INPUT: Fine Tune encoder data pin
* 6: To PCM1808(ADC, analog to digital) OUT pin
  - Digital audio input to the microcontroller
* 7: Teensy Audio Adapter Tx pin
* 8: Teensy Audio Adapter Rx pin
* 9: Unused
* 10: Display Buffer Group:
  - 74HC125 pin 1 (Output Enable)
  - 74HC125 pin 4 (Output Enable)
  - 74HC125 pin 10 (Output Enable)
  - Display pin 6 (chip select)
* 11: Display Buffer Group:
  - 74HC125 pin 2 (Channel 1 Input A)
* 12: Display Buffer Group:
  - 74HC125 pin 6 (Channel 2 Output Y)
* 13: Display Buffer Group:
  - 74HC125 pin 9 (Channel 3 Input A)
* 14: INPUT: Filter encoder clock pin
* 15: INPUT: Filter encoder data pin
* 16: INPUT: Tune encoder clock pin
* 17: INPUT: Tune encoder data pin
* 18: Teensy Audio Adapter SDA pin and Si5351A (clock generator SDA pin) (Serial data)
  - Connected to a pull up resistor
* 19: Teensy Audio Adapter SCL pin and Si5351A (clock generator SCL pin) (Serial clock)
  - Connected to a pull up resistor
* 20: Teensy Audio Adapter LRCLK pin
  - Left-Right clock
  - Indicates whether the current stereo audio sample is for the left or the right channel
  - One cycle per audio sample
  - Low: left
  - High: right
* 21: Teensy Audio Adapter BCLK pin
  - Bit clock
  - Synchronizes the data on the I2S (serial) data line
* 22: To Rx/Tx section
* 23: Teensy Audio Adapter MCLK pin
  - Master clock
* 24: Unused
* 25: Unused
* 26: Unused
* 27: Unused
* 28: To 20M Filter
* 29: To 15M Filter
* 30: To 80M Filter
* 31: To 40M Filter
* 32: PMC5102 DIN pin
    - DAC Digital audio input pin
* 33: Unused
* 34: Unused
* 35: To Key 2 section (CW)
* 36: To Key 1 section (CW)
* 37: To PTT section
* 38: To Mute section
* 39: Tune encoder SW pin

### Notes
* 74HC125: Quad digital buffer, used for the display
* PMC1808: Analog to digital converter
* PMC5102: Digital to analog converter

### Other notes
* Couldn't figure out where the blue arrow labeled Tx/Rx and for the 20/15/80/40M filters on pins 22 and 28-31 went
