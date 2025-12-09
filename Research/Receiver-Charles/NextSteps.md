So, after creating the prototype, and after presenting it at the Expo
# What are the next steps?
Specifically receiver and antenna-wise, we've been doing all of that analog. The next step would be to leverage the STM microcontroller to do the signal processing.

For example, we have the mixer that's doing the demodulating of the received signal, outputting the carrier wave & the audio from the received signal. Instead, we could directly pipe the received signal into the microcontroller, and digitally demodulate the signal, then output only the audio signal. This also would be a very similar story for the transmit circuit.

We also still have to figure out what was overloading the transmit circuit. When we created it, there was a decent amount of distortion of the transmitted signal. We suspected that it was either the input carrier wave or the input audio being too loud, thus affecting the modulated signal and distorting it. We may have to attenuate the carrier wave next time to get as pristine of audio as we can.

We still also have to make sure the antenna is as resonant as possible on the 10 meter band, and try to get the impedance as close to 50 ohms as possible to minimize reflections. All of our testing so far has been without the antenna, and just piping the transmit signal into a receiver, as we had no proper setup for an antenna, as we were doing all of the testing indoors. Next step would definitely need to be testing how far we can get with the transmit circuit hooked up to an actual antenna for 10 meters.

Given that we have the proper materials, we could also try and build our own low-pass and high-pass (or band-pass) filters. This would be a very good learning experience from applying filters theory (bode plots, filter components' value calculations) into an actual working filter for radio usage. Looking at the different types of filters, it's very likely we would use an active filter. However, a good first step would be to create a passive band-pass filter for frequencies around 28 MHz to 28.1 MHz.
