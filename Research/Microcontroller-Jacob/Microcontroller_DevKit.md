Research for the dev kit will begin from here: https://www.st.com/en/microcontrollers-microprocessors/stm32h7-series/products.html

* We will want to have a dual core board for better parallel processing capability
  * TODO: Expand on this here
* Minimum of 480MHz
  * TODO: Expand here
* Needs to be a dual core dev board for load splitting and smooth/seamless operation
* Double-precision FPU
  * Displaying and processing the frequency of the transciever will require the use of floating point numbers.
  * This will allow for the highest acuracy and precision of processing numberical frequencies.
* Co-Processor Type
  * 3 Options: None, Cordic, Cordic + FMAC
  * This would affect our ability to digitally process signals (with a DSP)
  * After some talk we have decided to contain our DSP purposes external from the STM32
  * This option holds no weight
* Needs dual bank flash
  * Dual bank flash allows the simulatneous code execution from one bank while allowing R/W operations to the other
  * This would be optimal in running the radio and doing other options with storage simulatneously such as saving the VFO to memory



Tasks for this week:
* Research how to implement a DDS (direct digital synthesis) module with the STM32
* Implement a basic UI system with the controller Alex will eventually give
* Continue dev board research
* Find something I like with the pixie kit lol
