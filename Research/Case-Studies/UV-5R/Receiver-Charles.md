# Speaker Amplifier
Similar to the ATS-20, the output from the SDR chip is amplified only to be able to drive the speaker. This is amplified by the op-amp denoted by U4, TDA2822. This then connects to the external speaker port as well. 

# Mic Circuit
The mic is an electret microphone embedded into the UV-5R. It is hooked up practically directly to the external mic input, as to turn on or off depending on if there's an external mic connected. For the internal microphone, it's powered by the 3.3V and corresponding current regulating resistor R71, with the output being piped into VOX and PTT for the microcontroller. The PTT is also connected to the PTT on the SDR chip to give it an indication when we're transmitting and when we're receiving.

# Receiver Process
Essentially the UV-5R has a very similar receiver process as the ATS-20. The signal received by the antenna immediately goes into an LNA, then gets piped into the SDR chip to do all the signal processing. The RF out then leaves the chip, and goes through another amplifier, and gets sent back out to the antenna.
## Filtering
Lots of filtering is done by analog components. Since most likely the SDR chips and other corresponding ICs and chips aren't built specifically to be used in the UV-5R, they don't have filters built into it. Especially for the transmit portion, as that would have to have an IC capable of handling 10W. So, the filters that are built into the radio are all analog, consisting of many capacitors, inductors, and resistors, at various values for the specific purpose.

To go more in-depth about the filtering, after the antenna, there are massive filters starting at L32 and L36. There are two to correspond to VHF and UHF receiving (depending on state given by the SDR/microcontroller), with varying values of each component depending on the band. This is then piped directly into the SDR chip for further digital signal processing, unmixing, and corresponding AF audio output

## FM Receiver
Specifically for FM, since the UV-5R can only receive FM, there is a separate, small chip dedicated to demodulating the received FM. It's name is the RDA5802, taking in the signal from the antenna after a few analog filtering circuits, and piping the received signal into the 

# CTCSS & DCS tones (and DTMF)
## Input
The microcontroller generates the CTCSS, DCS, and DTMF tones (pin 21). The microcontroller takes input for DTMF (which tone to create depending on specific key pressed), and CTCSS and DCS set by the user in the chip.
## Output
The corresponding tones generated due to the corresponding inputs are outputted from a pin on the microcontroller (pin 20 on MCU U6), and is inputted into the SDR chip (pin 32, RDA1846). The SDR chip then sends out the corresponding tone at the same time as the audio, and also handles the receiving and decoding of the tone with its message.
