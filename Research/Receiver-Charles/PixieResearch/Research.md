# Antenna
On the pixie kit, there is a female BNC connector to supply your own antenna
- Pixie kit build we did last year, we built a 40M dipole out of 16 gauge speaker wire. Worked well!
The pixie kit BNC connector is expecting a 50 ohm impedance from the antenna.

# Amplifier & Filter
There are two main transistors in the pixie kit.
The first one is directly connected to the crystal oscillator to function as generating the carrier wave for transmitting, and also to generate the reference signal for the receiver when it's switched into receive mode.
The second one is to amplify the output signal of the pixie kit to the ~500 millivolts (0.5 volts), and also function as a filter when receiving signals

Since pixie is direct CW conversion transceiver (uses one crystal for transmit and receive), there is a transmit/receive offset so that you can still receive signals when they are "zero beat" on you

# CW Key
On the pixie kit, there is a key slot for a 3.5 mm audio jack to connect your own key.
- This acts as a switch between transmit and receive mode
