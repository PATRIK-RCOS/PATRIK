# Power Supplies:

## Purpose:

A power supply is a critical component of an HF radio since it needs to provide a consistent, low noise output voltage that is consistent regardless of current draw. Some characteristics that must be considered are input voltage, output voltage, output current, power rating, regulation, ripple and noise, form factor, connectors, cooling, efficiency, and protection features. This report will cover considerations for different power supply topologies, comparisons to preexisting designs, and a recommendation for steps forward.

## Characteristics:

Input voltage varies depending on the load of a given circuit, but typically an acceptable standard provided by the National Electrical Manufacturers Association (NEMA) is ±10% of the rated voltage. Additionally, the North American Electric Reliability Corporation (NERC) maintains that the frequency should be standard, within ±0.02Hz. North America uses a nominal outlet voltage of 120VAC at a frequency of 60Hz, but both of these values are rarely what will be measured at an outlet. Fortunately, power supply design is tolerant of changes in voltage (assuming a regulated output), and the frequency is relatively stable. The design of this power supply will target operation from 120±10% VAC at 60±0.02 Hz. An additional consideration to make in the future would be to allow operation on other grid voltages, such as 240 VAC or higher.
Output voltage for amateur radio usage is typically 13.8 VDC due to the fact that a fully charged lead-acid battery is typically measured at that voltage. The tolerance for output voltage is typically ±15%, so the target output voltage will be 13.8±15% VDC.
The target power output of the radio is going to be 25 W. A significant amount of power however will be lost in the inefficiencies of the rest of the system, so headroom will be critical. Since there are no current figures for efficiency given the matrix of design choices that have yet to be made, this report will assume an efficiency of 50%. As such, the target power of the power supply will be 50W, meaning that it should be able to supply ~3.6A at the rated voltage of 13.8V.
Power regulation is the supplies ability to maintain an output voltage given a wide range of loads. This is heavily coupled with the range of output voltages, so the goal is to allow for an output voltage swing of 13.8±15% VDC regardless of load.
Ripple is one of the largest challenges of power supply design. Arrays of capacitors, inductors, and resistors, as well as voltage regulators are all possible solutions to decreasing ripple. While it is not entirely possible to eliminate ripple, reducing it to a point where it is no longer an issue is achievable. This design will target a maximum ripple of 10mV to minimize the impact of ripple on the design.
Form factor will be critical when designing this system, since it should be portable. At the moment, there are not any concepts for size and they will be determined later. The design will need to fit in a compartment on the radio device, so that is the only limitation.
The power connector that will be used are Anderson power poles, since it is a very common connector that many amateurs will already have, they have a variety of current specifications that all exceed the draw that this system will use, and it is the club standard for W2SZ.
Since the components have not been selected yet, temperature requirements will not be set. This system can optionally have a fan to cool the power supply, but that will be determined later.
This system is meant to be used by amateur radio operators, so many mistakes can happen. As a result, there will be a number of protection features as follows: Overvoltage protection, undervoltage protection, overcurrent protection, short circuit protection, and over temperature protection. This will ensure that no components are damaged at the fault of the power supply.
Input Voltage
120±10% Volts AC
Input Frequency
60±0.02 Hertz
Output Voltage
13.8±15% Volts DC
Current Output
~3.6 Amperes
Ripple
10 Millivolts



# Types of power supplies:

## Linear:

Linear power supplies can be understood through four main stages. At first, 120VAC is provided on the input of the power supply. All current is immediately fed through a fuse to protect the entire system. Additionally, a switch will be located after the fuse so that energy to the power supply can be shut off without consequence. Next the transformer steps the voltage up or down through magnetic induction. Transformers also isolate the primary coil from the secondary coil (galvanic isolation), which makes it much safer for human use. The output of the secondary coil is connected to a rectifier. Rectifiers are typically made of diodes and are critical to make the voltage positive on the output (as opposed to the alternating voltage, which swings positive and negative). An issue with bridge rectifiers is that diodes have a relatively constant voltage drop across them, which leaves with a lower output voltage. Using a split secondary transformer allows the power supply to only use two diodes on the output, which decreases the voltage drop by half. In other words, full wave rectification is possible with two diodes. There is still significant ripple on the output of the rectifier, so a large capacitor is necessary to smooth the output. A larger capacitor does a good job of removing noise from the rest of the system as well, and is often referred to as a bus capacitor. The main issue with adding large capacitors is that it is bad for the power factor. At this point, active power correction is needed to make the power supply more effective. Or alternatively, using a switched power supply. The output voltage of this system is also extremely susceptible to voltage drop due to loading. To improve this, a regulator would be a great addition. The two types of regulators are open loop and closed loop regulators. These work by consuming power to bring the voltage down to a constant point. Zener diodes are typically used to set a constant voltage drop regardless of current consumption, and it is used in conjunction with a transistor. A transistor is used to create a voltage follower, creating a small voltage drop similar to that of a diode. This is a fairly inefficient system since current is constantly being consumed by the resistor diode array. For a closed loop regulator, there is an op amp that measures the output voltage and corrects it based on a set of parameters. This also allows for voltage output control with a potentiometer.
Switching Mode
Switching mode power supplies are much more efficient. They begin by rectifying the input voltage to a positive value and filtering it with a capacitor. Next, a MOSFET turns on/off at a high frequency, generating a square wave. These power supplies operate at much higher frequencies, which allows for smaller transformers (either pulsed or flyback). These power supplies typically have 5 systems. The first is the EMC filter and fuse, the second is the rectifier and primary filter, the third is the driver and MOSFETs, and the fourth is the transformer and another rectifier, and the fifth is an output filter.
The EMC filter cleans the 60hz input by removing high frequency noise. The source of this noise is typically other devices on the circuit (like microwaves, computers, air conditioners, etc.). By filtering the input of the system, it makes it much easier to isolate the input to control the output more easily. Noise propagates through the system and is a negative characteristic of any power supply. Next, the rectifier converts the AC to a full wave rectified DC voltage, and a capacitor is placed in parallel to ground to smooth the voltage. At this point, there is a crudely rectified DC voltage, but significant manipulation to the signal still needs to occur. Next, transistors are used to pulse the input signal rapidly. It is worth noting that these transistors require consideration for heat dissipation due to the high-power throughput. A controller is used to switch the transistors on and off in a control loop to create a constant voltage on the output of the supply. If the voltage climbs to high, the control loop will decrease the duty cycle. If the voltage drops too low, the duty cycle will increase. This is happening much faster than a human could witness, however. This high frequency output drives the primary coil of the transformer. On the output of the transformer, there is a PWM controlled AC square wave, which is then filtered so that a constant voltage is approximated. The feedback from the output of the supply is measured through an optocoupler in order to keep the high voltage and low voltage sides isolated. It is critical to isolate the high voltage side from the low voltage side. There is also a current feedback signal to prevent the current output from going above rated values.

How to decrease noise in a power supply?  
Why are multiple capacitors used in parallel in a power supply?  
==> Output capacitor smooths output voltage from output  
==> Parasitic elements: undesirable effects of electrical components  
==> Parasitic effect of capacitors: Equivalent series resistance (ESR)  
o How does ESR impact performance?  increases ripple despite capacitive effect  
o To improve, place multiple capacitors in parallel -> total impedance decreases, thus minimizing impact of ESR. Two similar capacitors in parallel will decrease ESR by half.  
==> Route power traces through decoupling capacitor pads  
==> Make power traces thicker (positive voltages, negative voltages, ground)  
==> Make large ground pours to minimize impedance, do not allow ground pours to act as antennas  
==> Ferrite can be used to support decoupling  
==> Power supply frequency can couple with signal frequencies -> Choose a switching frequency that is rejected by the rest of the system. LDOs can be used to decrease coupling.  
==> Frequency planning!  
==> Capacitance multiplier??  
==> Linear regulators are not great at decreasing ripple (given 10s-100s mV of ripple)

Purpose:

Pixie kit power supply techniques.

The pixie kit uses multiple power supply techniques. 

First, there is a power input (typically from a battery or ac rectifier) which is connected via pins.

For designs that do not use a battery, ac power is accepted and is rectified on the circuit using a built in bridge rectifier.

For this design, it prevents the system from reverse polarity as well. 

For designs without a bridge rectifier, a Zener diode is used across the power supply.

This Zener diode allows for transient voltage suppression, with all current going straight to ground under high voltage situations.



Different components are fed through the bridge rectifier. This includes the oscillator, power amplifier, and audio amplifier. 

Since these circuits experience intermittent transients, decoupling capacitors are used to prevent significant voltage drops.






# References:

https://www.analog.com/en/resources/technical-articles/smart-hardware-eng-can-easily-design-power-supp.html  
https://ww1.microchip.com/downloads/en/appnotes/01114a.pdf  
https://www.youtube.com/watch?v=UTetQhGyUVg  
https://www.youtube.com/watch?v=cK5WLMFSGnw  
https://www.westmountainradio.com/new-ham.php  
https://www.youtube.com/watch?v=Bt1hi9C1Upk  
https://www.youtube.com/watch?v=fKGyVSKm2Dc  
https://www.youtube.com/watch?v=wopmEyZKnYo

