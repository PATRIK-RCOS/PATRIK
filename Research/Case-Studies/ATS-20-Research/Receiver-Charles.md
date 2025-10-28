# Receiver
## Antenna
An antenna is supplied with the radio, although it is not sufficient enough to cover all HF bands.
This specific antenna is a telescopic antenna that connects directly onto the BNC slot on the radio, functioning as a length-adjustible verical antenna.

## RF Input
RF input from the antenna is denoted by the Conn_Coax symbol on the right of the schematic, with the outer shield of the coaxial cable going directly to ground. 
Directly after the coax connection, there is a component that is specifically designed to protect against ESD, being static electricity or stray voltage that may accumulate over time. This only kicks in once a higher-than-average spike of voltage is detected. Otherwise, this component is not active in the circuit.
After the ESD protection component, there is a switch that toggles between two different pins on the chip, FM and AM respectively, as the chip has different pins for AM and FM. The capacitors following the relative AM and FM wires are to filter out DC voltage and only allow the oscillating sinusoidal input from the antenna through, making sure the stray DC voltage doesn't harm the chip itself.
Interestingly, in between the AM and FM pins of the chip is a resistor labeled "DNP". This means "Do Not Place", and is most likely a placeholder resistor for the circuit designer to easily place when redesigning. 

## Crystal Oscillator
The chip itself is powered by 3.3V DC, which is supplied from the power supply circuitry above.
Connected to pins 19 and 3 is the crystal oscillator circuit. This crystal oscillator is not for generating the carrier frequency sine wave, but rather to act like a timer for the chip.

## Output
Pins 23 and 24 are the respective audio outputs of the data from the received RF signals. This chip outputs both a left and right audio as to make stereo sampling easier.

# Amplifiers
The ATS-20 does all of the RF mixing, amplifing, and carrier wave separation inside of the chip itself.
In the amplifiers section, the input is the respective right and left audio outputs from the chip after mixing and separating the audio from the carrier wave. Each audio input goes through separate non-inverting op-amps, including inductors and capacitors to filter out DC and certain frequencies, which both then go into the headphone jack.
The audio fed into the headphone jack is also routed into another audio amplifier, which amplifies the audio to the point where it can physically drive a speaker, terminating at the speaker connections on the schematic.
