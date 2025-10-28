# Schematic Link
* https://www.qrzcq.com/pub/RADIO_MANUALS/BAOFENG/Baofeng--UV-5R-Schematic.pdf
* Breakdown: https://www.allaboutcircuits.com/news/teardown-tuesday-baofeng-amateur-radio-transceiver/
* Microcontroller is located at position U6

# Microcontroller Specifications
* The microcontroller model is EM78P568
* Architecture: 8-bit RISC 
  * To contrast, the STM32H7 line is entirely 32-bit
  * This means that any computation that requires values over 255 (2^8 for 8 bits) will require greater overhead and will be much more inefficient
* Clock speed: max 17.913 MHz
  * Much better than the average arduino which makes it acceptable for text-only user interfaces
* Flask memory: 26 Kb
* RAM: 1 Kb

# Notes
* The microcontroller's main purpose is to interface with other components and control modules to do things like:
  * Adjust VFO
  * Interface with stored flash memory for using channels and other settings
  * Manage input (buttons, volume rotary encoder, etc.)
  * Manage output (LCD, speaker volume output, etc.)

# EM78P568 Pin Notes
  * Pin 9: Output to LCD controller register select
  * Pin 10: Output to LCD controller chip enable pin
  * Pin 16: Output for recieved audio (post-processing?)
  * Pin 17: Input for recieved audio
  * Pin 18: Input for microphone audio
  * Pin 19: Output for microphone audio (post-processing?)
  * Pin 21: Output of the internal tone generator for CTCSS / tone squelch
  * Pin 29: Oscillator input
  * Pin 30: Oscillator output
    * Piped back into pin 29 (OSCI) after a filter? (ask)
  * Keypad button array
    * Standard button grid array, used on practically all input systems (ex. keyboards)
    * P90,P91,P92,P93,P94 - Grid x-axis
    * PB5,PB6,PB7,PC2 - Grid y-axis
    * Pins used: 1,2,3,4,5,6,7,8,44

