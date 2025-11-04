# Mixer
In this radio, there are 2 things that perform the function of a mixer, the QSD (Quadrature Sampling Detector), and the QSE (Quadrature Sampling Exciter). The similarity if that there are 2 signals that are 90 degrees out of phase with each other, one is the I (In-Phase), and the other is the Q (Quadrature).

## QSD
What the QSD does is that it takes the SSB singal into the IC, which is mixed with the clock (from the Si5351A-B-GT chip), which splits the I and the Q signals. If the clock is set below the carrier, it is LSB, and vice versa. For CW, you can choose either sideband, and it will do the same thing except the output will just be the tone.

## QSE
What the QSE does is that it will take an I signal from the tip and a Q singal from the ring, and it will combine these signals together along with the clock signal. Because of the 90 degree offset, the mizer will cause destrictive interference with the unwanted sideband and the carrier, supressing them and only letting the wanted sideband through to be transmitted. If the clock is set below the carrier, it is LSB, and vice versa. For CW, you can choose either sideband, and all that is sent is a tone (typically 700 hz).