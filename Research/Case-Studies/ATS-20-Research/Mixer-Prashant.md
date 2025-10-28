# ATS-20 Mixer

The ATS-20 does not have a mixer, it uses direct signal pricessing inside of the SI4735-D60/SI4732-A10. What is does it it uses an ADC (Analog-Digital Converter),  which takes a digital sample of the analog signal at 32.768 kHz, which is fed into the chip to process the signal digitally.

It also doesn't transmit, so there is no mixer there.

## Sources

https://github.com/Emmemad/ATS-20_Schematic
https://resources.pcb.cadence.com/blog/clocking-adcs-in-mixed-signal-systems
