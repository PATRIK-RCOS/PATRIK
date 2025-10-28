The T41-EP-SDT is comprised of 8 main, separate circuit boards.
The main ones corresponding to the receiver are covered below in their corresponding sections.
# Main Board

# PA

# QSD

# QSE Exciter

# Xmit Filters
These are various filters depending on the band selected. According to the schematic, there are control pins that are switched between depending on the state the user chooses. Each of these pins goes to their corresponding filters for each band. These filters are both used for transmitting and for receiving. Each filter is connected to one another, and there are certain paths that allow the signal to pass through to the filter it needs to go to. For example, if the radio was switched to the 10m band, the switches would form in such a way that the wire goes only to the 10m filter. The signal practically goes past every other filter, and only goes through the filter it's supposed to be going through. 
There's also corresponding switches/relays at the bottom, which switches to either the PA (Power Amplifier) for TX, or for Receive for RX. PA is the power amplifier input to go through the filters to eventually get to the antenna, and the output from the schematic into the receive board has a final filter before leaving the Xmit Filters board.

# Source
https://github.com/peterbmarks/T41-EP-SDT/tree/main/Schematic%20PDFs
