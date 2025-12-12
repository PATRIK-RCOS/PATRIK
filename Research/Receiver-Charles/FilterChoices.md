In order to maximize the ease of use of the radio, as well as make the user experience much better, we could add more filters on top of the filters that we currently have.

We currently implemented a band-pass filter in order to keep the receiving and transmitting signals on-target with what the radio is supposed to actually receive and transmit. However, there are some cases in which other filters are not only nice to have, but also essential.

For example, let's assume we created the radio as a portable system and it fully works. We move to a different area of the US, or try to operate from a remote location, but there is some kind of interference on the frequency that we are trying to listen.
While this wouldn't be such a massive problem for radios you purchase off-the-shelf, since practically all modern radios nowadays include a VFO (variable frequency oscillator), allowing you to switch frequencies, this is a huge problem for our radio since our prototype is only tested to be working on one specific frequency. While we have the capability to change the internal radio oscillator itself to a different frequency, we haven't fully tested this and we can assume our first prototype works best on that one specific frequency.

So, while attempting to receive a voice signal (3 kHz wide bandwidth), what if there's random interference in the form of some mechanical humming from a man-made object (QRM)? If it falls within the 3 kHz wide bandwith that we are listening to, then this will mean that it'll constantly be heard, especially if it's powerful enough, then it'll be louder than the voice signal or be as loud, making it much harder to understand the transmitting station.

Thus, this is a significant problem. Fortunately, there are some different ways we can attempt to counteract this problem. This comes the topic of this document, which is different types of filters!

The most commonly used filter to remove interference at a known frequency is a notch filter. Specifically one kind of notch filter that we can make, and that we have the knowledge to make, is the band-stop filter.
A band-stop filter is essentially the opposite of a band-pass filter. We already have a band-pass filter integrated into the radio, to only let signals be transmitted and received in the 28.000 -> 28.100 MHz range. However, a band-stop filter does the opposite, which is completely block out all signals within the specified range.

The only design issue would be that we would need to change the values of the filter in order to vary where the filter is actually removing noise from. While it would be great to have any notch filter, we ideally want the filter to be adjustible to where the interference is coming from. Otherwise, if the interference is on a frequency that is not covered by one of the band-stop filters, there would be practically no way to remove that interference.

Thus, we could digitally implement a band-stop filter, or also known as a type of notch filter, in order to remove various sources of interference. In addition, we can also digitally implement different types of filters. For example, we could create a digital filter that you can vary the bandwidth of signals you are listening for.

This type of filter, changing the width of listened signals, if very important for hearing specifically the station you are tuned to. For example, if the bandwidth you are listening to is about 3 kHz, as stated before, if there's someone else talking within that bandwidth, you will also hear them. Lessening the width that you're listening to to about 500 Hz would allow for much better isolation of that one signal. This is also especially important for contests, as there are very notable contests that happen every year that have lots of people transmitting on the air at once, using any free space there is on the band as possible. Thus, you might be listening to even four stations at a time, even if you really don't want to.


This was practically just a rant on various choices for filters that could be implemented in the future.
Especially since we're looking to digitally implement these filters, it makes it so much easier to actually use, and once we have a filter digitally implemented, we could easily implement the other kinds of filters.

All in all, this would make the radio much more user accessible, much easier to use, much easier to receive, and actually work for high-stakes contests with lots of people transmitting near your station at once.
