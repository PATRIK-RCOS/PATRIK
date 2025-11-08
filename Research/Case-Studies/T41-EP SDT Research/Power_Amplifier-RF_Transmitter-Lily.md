# T41-EP SDT Transceiver

## Power Amplifier for Speaker

### Function
- 20 watts amplifier as said previously
  > mentioned "fuzzy QRP" as can change it
  
### Parts:
- uses 4 IRF510 transistors, heatsink so keeps it from overheating too much
- changes most impedances to 50 ohm out
- voltage requirement is 12 V for 5V DC and 3.3V DC
  > BUT if  want 20 watt for RF power amp then need 25V DC in as well
- has 0 db gain for the amp.

### LOCATION: 
- main/digital for some components
- possibly attached to heatsink in some cases


Sign up for rcos for next semester 

## RF Transmitter/RF Amplifier

### Function
- makes and sends radio waves
- modulates the audio into a rf carrier, amplifies it, and is sent by the antenna
- for this radio specifically:
  - it is controlled by and functions by the Teensy 4.1 microcontroller (does the sending and receiving for the radio)

### Parts:
- ssb exciter board : makes the ssb's modulation
- filters for low pass so sending on intended frequency
- amplifier of 20 watts (not fixed)
- swr meter is visible
- digital controls
  
### LOCATION: 
- ssb Exciter board
- main board
- filter board


## Sources
- [https://ai6ym.radio/t41-ep-sdt/](https://ai6ym.radio/t41-ep-sdt/)
- [https://github.com/peterbmarks/T41-EP-SDT](https://github.com/peterbmarks/T41-EP-SDT)
- [https://4sqrp.com/kits/T41/4SQRP%20T41%20Assembly%20Manual%20-%201-5-24.pdf](https://4sqrp.com/kits/T41/4SQRP%20T41%20Assembly%20Manual%20-%201-5-24.pdf)
