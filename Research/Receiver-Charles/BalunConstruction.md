While creating the balun, I created some notes for future people that want to create a balun. 

In some connectors, specifically in one of the connectors used to create the balun, the dielectric inside of the connector might spin. While this might seem inconsequential, once I fully finished putting together the balun, when I was screwing on an adapter to change the connector to BNC female, the dielectric inside spun while screwing the connector on and resulted in the connected enameled copper wire to the main pin of the connector breaking off. I had to solder on an extra 26 gauge piece of wire to the balun and the connector in order to securely connect it back again.
Thus, when creating a balun, make sure your connectors are of good quality and have no defects, especially if the dielectric spins. This could have been much worse, as I could've fully finished the enclosure, put the adapter on, and thought everything was completely fine until I continuity tested it, and realized it broke off. 

When creating the balun, I chopped off a little too much wire than I was supposed to. Thus, it is a very close fit and tere is very little to no stress relief if both connectors are tugged in opposite directions. This is not ideal, especially since the solder to the ground side of the connector isn't the most stable. Thus, always remember to keep extra wire so there is strain relief. In this case, extra wire won't hurt the balun at all, since the wire is coated. If anything, you could even fold the wire to make a manifold that expands when the connector is pulled.

For this balun, I used 5 turns on one side, and 5 turns on the other to keep the 1:1 match. I also wound the wires together using a drill and pliers, so that they stay together and is much easier to run rather than two separate wires around the balun.


Below are some pictures of the finished product, in order of progression of enclosed the casing is.

<img src="IMG_4802.jpg">
<img src="IMG_4805.jpg">
<img src="IMG_4807.jpg">

# Testing
In order to test the balun to see if it actually works, we can use a nanoVNA (Nano Vector Network Analyzer) in order to analyze the impedance. Using the nanoVNA, we can also measure the insertion loss and the signal attenuation. 
To test it, you need to connect the inner conductor of the balun in series with port 1 and port 2 of the nanoVNA. You would also short both shields (outer conductors) in order to get a consistent measurement, and connect it to ground. Then, the nanoVNA should display the magnitude of an input signal for the frequency range you specify. Ensure the frequency range includes the frequencies you want the balun to work oN!
Once you've connected it and you see a magnitude measurement, as long as the attenuation is about ~20-30 dB, that is "good enough" for a balun.

If the attenuation is not in that range for the frequency you want the balun to work on, you can change the lengths of the balun wires. If there is too much spare wire on the ends of the balun, that could throw off the measurements. Also, if the attenuation is way off, try adding or removing one turn. Doing this over and over again, and trying to optimize the attenuation at the intended frequency, will eventually get your balun tuned to where you want it to work the best.

Unfortunately, this is ideal conditions. The impedance of the common mode choke, or balun, will change once there is an actual load presented at the end, such as an actual antenna.

In that case, the measurements will be off and the impedance won't be exactly 50 ohms, but it wouldn't vary too much in which it wouldn't even let any power through. Worst case, it attenuates the output signal by a few dB. Not ideal, but also not the worst.

Note: lots of information taken from this video.
https://www.youtube.com/watch?v=E9F7Aot_AFg
