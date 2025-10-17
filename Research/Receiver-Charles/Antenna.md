## Specifications:
- Must handle 25 watts 100% duty cycle maximum
- 10M HAM Band

## Final Design Choices:
Center fed dipole
- with a 1:1 balun
16 gauge wire
5.5 feet above ground
- still tentative..

### Why a Center-Fed Dipole?
A center-fed dipole is one of the easiest antennas to create. The antenna itself requires just a coaxial connector, and you solder one length of antenna wire to the center connector, and another to the shield, and you have a working antenna.
I chose to use a dipole because all other antenna types that use solid metal are often too long, cumbersome, and also expensive & hard to manufacture at the HF bands. We can see lots of vertical and hamstick antennas in the VHF and UHF range, but not many past that. If we were to put a vertical up for the 10M band, it would require a 5M ish vertical antenna, with a ground plane, and most likely guide wires to keep it standing. There are 10M ham-sticks you can purchase online, but manufacturing a hamstick would require a lot of time, extremely long compared to the dipole manufacturing process.
In comparison to other wire antennas, specifically focusing on differing feed points for the antenna, practically every feed point rather than the center requires an additional part to be manufactured. This would be the impedance transformer. The antenna ideally would have an impedance of 50 ohms, as the coaxial cable feeding to the antenna also has an impedance of 50 ohms. The dipole, when fed in the center, has an impedance very close to 50 ohms that can be easily fine-tuned by trimming either side of the antenna. On the other hand, an end-fed dipole has an impedance of about 2500 ohms at the feed. This means that end fed dipoles require the impedance transformers to convert the 2500 ohms impedance down to 50 ohms (unless you can find a 2500 ohm coaxial cable... but you'd still need to have your radio accept 2500 ohms impedance!). In this specific case, the impedance transformer has to be a 49:1 ratio to get down to 50 ohms. Similar to the end fed dipole, other dipoles not fed in the center will have an impedance that is slightly, or very, off of 50 ohms and still will require an impedance converter, although of a smaller ratio. As stated before, a center-fed dipole has an impedance very close to 50, not requiring an impedance transformer, and thus the better place of feeding for dipole antennas for our case.

### Why a 1:1 balun?
A balun removes common mode currents that accumulate on the antenna. These common mode currents can impact performance of the antenna, as well as potentially damaging the internal radio circuitry itself if no other protections are in place. A 1:1 balun is very simple to create, and isn't fully necessary, but is ideal.

### Why 16 gauge wire?
16 gauge wire is actually overkill for this project. However, it is practically guaranteed to handle 25 watts. Other amateurs often use 16 gauge speaker wire to run 100 watts! It's also that the wire can hold up its own weight more reliably than smaller gauge wires, especially since we are going to be elevating it over ground. Not only does it have to hold its own weight, but it also has to handle weather conditions when assembled and put up for a longer period of time.

Mast design?
- Need to elevate above ground
