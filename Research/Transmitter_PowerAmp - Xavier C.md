Transmitter – HF 10m 

Considerations for possible input waveforms – work with Hisen to understand the current landscape. 

14.060 crystal oscillator as the base for a Variable Frequency Oscillator. -- very common, easy base frequency to shift 

Consider direct digital synthesizer – square wave input, will need to “soften up” 

28-29.7 MHz requirement (ham 10m band) 

IEEE citation. 

To do:  

research the existing transmitter & power amplifier architectures 

Xiegu digital input architecture if I can find it, alt. Look for forum works (sdr/digital synthesizers) 

Research how the digital synthesizer input is filtered enough to not trigger horrible sideband by various SDRs 

Pixie Kit analog input architecture – needs to be modified to accept a higher wattage linear amplifier chip 

Research tuner interface –the current one I know off the top of my head is Xiegu’s, refer to next point for possible alternative standard interfaces. 

Look up ISO/IEC standards for these components. We may be able to use those as a guiding star. 

Note to all: If we want to make this radio cross-compatible with other products, we need to avoid falling into the “accidentally proprietary” routine, where we create things to interface with our own things. By researching and building according to existing ISO/IEC/FCC standards, we can ensure maximum cross-compatibility of firmware, hardware, and external counterparts. 

 

 

 

 

 

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

 

Transmitter Architecture 

Goal: to produce a stable, high-power radio frequency (RF) signal. The specific design varies depending on the desired power output and modulation mode (AM, SSB, CW, or digital).  In this case, we strive for SSB and CW. 

 signal generation (oscillator) 

Variable Frequency Oscillator (VFO) 

Digital synthesizer (square-formed analog waves) 

frequency multiplication,  

modulation,  

power amplification