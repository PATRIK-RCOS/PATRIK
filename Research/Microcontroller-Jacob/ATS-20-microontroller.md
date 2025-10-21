# Microcontroller Model
Arduino Nano v3
* ATmega328 microcontroller chip

# Microcontroller Specs
| Type   | Detail |
| ------ | ------ |
| I/O    | 14 pin |
| Flash  | 32 KB  |
| EEPROM | 1KB    |
| Clock  | 16MHz  |

# Notes
* EEPROM is used for non-code non-volatile memory
* 16MHz is suitable for a semi-static text based interface but in no way can handle graphics
* Inputs: 8 buttons and a rotary encoder
* Buttons
  * 2x Band control (next/previous band)
  * 2x Volume control (up/down)
  * Step (change how much the rotary encoder increments/decrements the frequency by)
  * BW (change bandwitch: narrow/wide)
  * AGC (toggles automatic gain control)
  * Mode (switches FM and AM receive)
* Rotary Encoder
  * Adjusts the frequency per the set step value
