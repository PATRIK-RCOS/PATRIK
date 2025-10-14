# Research for Dev Kit Boards
Research for the dev kit began from here: https://www.st.com/en/microcontrollers-microprocessors/stm32h7-series/products.html

# Filtering out the Bulk
On the ST website, there are 133 listed dev boards in the STM32H7 family. Because of this, we must filter out the bulk of the dev boards into a list that suits our needs.
* We will want to have a dual-core board
  * Better parallel processing capability
  * Load splitting and smooth/seamless operation
* Double-precision FPU
  * Displaying and processing the frequency of the transceiver will require the use of floating-point numbers.
  * This will allow for the highest accuracy and precision of processing numerical frequencies.
* Co-Processor Type
  * 3 Options: None, Cordic, Cordic + FMAC
  * This would affect our ability to digitally process signals (with a DSP)
  * After some talk, we have decided to keep our DSP purposes external to the STM32
  * This option holds no weight
* Needs dual bank flash
  * Dual bank flash allows the simultaneous code execution from one bank while allowing R/W operations to the other
  * This would be optimal in running the radio and doing other options with storage simultaneously, such as saving the VFO to memory
* Because Chrom-ART 2D graphics acceleration is available, it would help create a user interface that is free of lag; therefore, we will require it
  
With these options set to filter the majority of the dev boards, we are left with 2 boards that more closely match what we want out of the STM32H7 family.

# Deciding Between Two Chips
* Differences between the two: STM32H745ZI vs STM32H755ZI
  * The only difference between these two models is the inclusion of a much greater support for cryptography and security features with the 55ZI.
  * The 55ZI includes cryptography types: AES, DES/TDES, SHA-1, SHA-2
  * Both models include: TRNG
  * [Chip Prices](https://estore.st.com)
    * STM32H745ZIT6 current chip price: $11.05
    * STM32H755ZIT6 current chip price: $12.52
* Even though there are unused features on the 55ZI and the prices are very similar, with the 45ZI beating the 55ZI by a small margin, the availability of 45ZI dev boards is greatly limited, with many listings having a higher price than the 55ZI.
# Decision
* This leads us to the decision to go through with implementing our project on an  **STM32H755ZI** dev board. 
