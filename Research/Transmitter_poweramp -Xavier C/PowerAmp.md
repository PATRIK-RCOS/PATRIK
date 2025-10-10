Transmitter – HF 10m 

Considerations for possible input waveforms – work with Hisen to understand the current landscape. 

14.060 crystal oscillator as the base for a Variable Frequency Oscillator. -- very common, easy base frequency to shift 

Consider direct digital synthesizer – square wave input, will need to “soften up” 

28-29.7 MHz requirement (ham 10m band) 

To do:  

research the existing transmitter & power amplifier architectures 

Xiegu digital input architecture if I can find it, alt. Look for forum works (sdr/digital synthesizers) 

Research how the digital synthesizer input is filtered enough to not trigger horrible sideband by various SDRs 

Pixie Kit analog input architecture – needs to be modified to accept a higher wattage linear amplifier chip 

Research tuner interface –the current one I know off the top of my head is Xiegu’s, refer to next point for possible alternative standard interfaces. 

Look up ISO/IEC standards for these components. We may be able to use those as a guiding star. 

Note to all: If we want to make this radio cross-compatible with other products, we need to avoid falling into the “accidentally proprietary” routine, where we create things to interface with our own things. By researching and building according to existing ISO/IEC/FCC standards, we can ensure maximum cross-compatibility of firmware, hardware, and external counterparts. 


\r

Power Amplifier – Chips and matching circuits, specs, etc.  

QRPGuys 10W  10m CW/SSB linear amplifier: https://qrpguys.com/cw-ssb-10w-amplifier 

SM0VPO 10W 10m linear amplifier (another ham): https://www.sm0vpo.com/tx/15w-pa.htm 

CHIP 

LM317M 

 

MFG 

TI 

LINK 

https://www.ti.com/product/LM317 

Specs 

 

 

 

Existing use cases & links 

 

https://next.gr/uploads/135-11530.png 

 

 

 
 

Alternative ()PUSH-PULL DESIGN:  

Uses MOSFETs, such as the IRF510 

We can use NPN transistors for this as well,  

https://schematicsforfree.com/files/Radio%20&%20Communications/Transmitters/Circuits/High%20Power%20HF%20Linear%20Amplifier.pdf 

These power amplifiers are intended to be placed between the transmitter and the antenna.  
