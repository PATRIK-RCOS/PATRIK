So, after creating the prototype, and after presenting it at the Expo
# What are the next steps?
Specifically receiver and antenna-wise, we've been doing all of that analog. The next step would be to leverage the STM microcontroller to do the signal processing.

For example, we have the mixer that's doing the demodulating of the received signal, outputting the carrier wave & the audio from the received signal. Instead, we could directly pipe the received signal into the microcontroller, and digitally demodulate the signal, then output only the audio signal. This also would be a very similar story for the transmit circuit.

We also still have to figure out what was overloading the transmit circuit. When we created it, there was a decent amount of distortion of the transmitted signal. We suspected that it was either the input carrier wave or the input audio being too loud, thus affecting the modulated signal and distorting it. We may have to attenuate the carrier wave next time to get as pristine of audio as we can.

We still also have to make sure the antenna is as resonant as possible on the 10 meter band, and try to get the impedance as close to 50 ohms as possible to minimize reflections. All of our testing so far has been without the antenna, and just piping the transmit signal into a receiver, as we had no proper setup for an antenna, as we were doing all of the testing indoors. Next step would definitely need to be testing how far we can get with the transmit circuit hooked up to an actual antenna for 10 meters.

Given that we have the proper materials, we could also try and build our own low-pass and high-pass (or band-pass) filters. This would be a very good learning experience from applying filters theory (bode plots, filter components' value calculations) into an actual working filter for radio usage. Looking at the different types of filters, it's very likely we would use an active filter. However, a good first step would be to create a passive band-pass filter for frequencies around 28 MHz to 28.1 MHz.

# Balun
In addition, we can also integrate the balun directly into the 10 meter dipole. The only drawback is that it would make the center enclosure with the antenna connector a little bulkier. However, it would allow us to use only one cable and have both the balun and antenna in one enclosure for the most optimal antenna experience, receiving, and power output.

If we decide to do this, anothre big consideration that we have to do is waterproofing the enclosure, especially if it's going to be kept up for a while. This also includes the antenna itself, because if we leave the antenna out in its current state, then it will very likely become water damaged over prolonged weather cycles. For these use cases, we could choose more weather-proof connectors (Type-N), or try and limit the amount of spaces that water could intrude in general. 

We could move from 3D printing the enclosure to getting a metal enclosure milled out for ideal weight handling, stability, and actual fit. This, of course, will cost us some money as it would be pure metal. In addition, we have to make sure none of the components are shorting due to the metal enclosure. We would have to figure out standoffs from the metal enclosure in order to space the circuit boards and the sensitive components from touching the grounded enclosure.

# Far-Into-The-Future
Looking much farther into the future, after we've fully implemented the digital signal processing, we can do more things digitally.
Morse Code is commonly used for radio communication, as it's a much more power-effective communication method. Instead of having to modulate a voice signal over ~3 kHz (for intelligibility reasons), morse code is simply an on-or-off signal. Thus, all of the power that was originally spread out over the ~3 kHz is much further condensed.
In addition, even if there is distortion to the signal, it'll still be able to be decoded.

Thus, not only am I recommending that we add morse code capabilities in the future, but we could also add a program that runs in the radio that automatically decodes received morse code transmissions. There are already radios that incorporate these translations already, but being able to make the program ourselves (and maybe even optimize it!), then adding it into the radio, would be perfect.
