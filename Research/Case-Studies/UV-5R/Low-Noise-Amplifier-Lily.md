# Power Amplifier Transmit (Speaker)
- not in the UV-5R
- can be added externally to increase output signal strength
- there is an audio amp. used for speaker
  - the chip TDA2822
  - this is a dual audio power amplifier

# RF Transmitter (RF Amplifier)
- power output: max 4W, low 1W
- the RF Transmitter is based off the RDA1846 chip and the MB95F013K chip
  - MB95F013K chip controls/programs the RDA1846 chip given user input thru button interface
  - RDA1846 chip + 3 additional external transistors functions as the transceiver
    - the 3 external transistors help as a power amplifier since no internal
    - DTC114EE then 2SB624 then MOSFET(2SK3078A)
- no RF amplifier specifically for Transmitter in the UV-5R
- can be added externally like the power amplifier for speaker
