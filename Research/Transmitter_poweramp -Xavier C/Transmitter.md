Transmitter Architecture 

Goal: to produce a stable, high-power radio frequency (RF) signal. The specific design varies depending on the desired power output and modulation mode (AM, SSB, CW, or digital).  In this case, we strive for SSB and CW. 

 signal generation (oscillator) 

Variable Frequency Oscillator (VFO) 

Digital synthesizer (square-formed analog waves) [selected]  

frequency multiplication,  

modulation,  

power amplification  

For DDS: 

https://www.qsl.net/yo4hfu/TCVR_HF_DDS.html 

We are using a DDS, but we will need a different architecture than using regular crystals to avoid logic hazards 

I will stick to KiCAD in designing the initial SPICE simulations for now, as well as find a solution to convert I/Q (in-phase (I) and quadrature (Q)) signals from the microcontroller to something ingestible by the DDS. 

Find a suitable DAC that can help smooth/process the output signal better than the DDS we are using 

Carrier Oscillator (or VFO): Generates a stable, high-frequency carrier wave. 

Keyer: This is a simple switch, often a hand-operated Morse code key or electronic keyer, that turns the carrier oscillator or a subsequent amplifier stage on and off. 

Driver/Buffer Amplifier: Provides isolation between the oscillator and the power amplifier, and a small amount of amplification. 

Power Amplifier (PA): Increases the power of the keyed RF signal to the desired output level for transmission. 
