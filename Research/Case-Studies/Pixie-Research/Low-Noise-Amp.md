# Low Noise Amplifier Research for Pixie Kit

## What Low Noise Amplifier Does/Is:
- Low Noise Amplifier is also commonly referred to as a LNA, Recieve Amplifier, Pre-Amplifier or PA/LNA(a connecting of a Power Amp. and Low Noise Amp.)
  
- amplifies low power signals 
	- makes weaker signals or signals getting drowned out by distortions or interfence, more clear 
    - this is done without any huge impact on the SNR (signal to noise ratio)
		> with the LNA no extra noise or disturbance will be added too significantly so the SNR will be maintained
        > the SNR is best to be kept stable as otherwise:
          - could make other noises seem louder than they actually are (magnifies noise)
          - could impact the receiver and its abilities (specifically displaying lower noise or weaker signals - very crucial part of operations)
    
- SNR (signal to noise ratio)
    - signal power to noise power
    - signal power : noise power
    - shows the max amount of data it is capable of transmitting although this can be altered or impacted by other factors such as which band radio is operating on
    - a HIGH SNR is desred, low means operation will not be as great (hearing other stations/signals clearly or at all)
      > high SNR = signal is clear
      > low SNR = signal is obstructed and unclear
      
- should consider the gain, RF input, and noise when looking for a LNA
    - these are important factors that influence the performance of the reciever 

- high gain ==> weak signals get strong ==> high level signals ==> cause harmonics (not good for operating and finding/understanding signals coming in)

- low noise is preferred so can find weaker signals among the potential distortions that could occur

- LNA can help prevent feed line loss by attaching a LNA in-between the antenna and receiver 
	- LNA's input = antenna & LNA's output = receiver input
	- the LNA's added gain with help make the loss less noticeable

- many categories of LNA's 
	- (circuit) Common Source 
	- (circuit) Common Gate

	- (stage count) Single-Stage
	- (stage count) Multi-Stage

	- Specific to a Frequency
	- Specific to be Low Power Devices
	- Specific conditions such as temperature
	- Specific Wide/Narrow Band
	- Specific LNA's for other uses (for more unique projects or rigs/setups)

## Applying to the 10 meter HF Radio for PATRIK:
- LNA not really usually put in 10 meter HF radios
  > but can still be useful if some factors are less than desirable:
    - if the antenna is not as powerful or resonant so the signal is weak
    - if the distance of coax cable is too long from radio to antenna = signal loss
    - if signals coming into the radio are just not good
      
- potential options to buy for 10 meter HF operations
    - [Lana HF v2 - Ultra Low-Noise LF, MF & HF Amplifier (LNA) Module for RF & Software Defined Radio (SDR). Wideband 50kHz-150MHz Frequency Capability with Bias Tee & USB Power Options](https://www.amazon.com/Lana-HF-Low-Noise-50kHz-150MHz-Capability/dp/B0BBSPDJBG/ref=sr_1_19?dib=eyJ2IjoiMSJ9.L5ucGNN3Pdy74YRb2g9q8tZvG8mDnvuTtVVgAgNlx1j55dVgR7DWRaXXZb2k8Py8-mT5AO8e-tPxBRGKUOGsH2loHGIfmgXlRs5FXPDa6TxWlrbYM7-0TCGXYJSkYSdvMqOllgzEIyUvNVaYqk0i-XA1c-qfH8vPfGhOfm9FZH_NlwkXeWLqc7scHCW67s-XTfzqpLvteXbzRenhnK5oPtgqhOwrE2Jq7TmfrLFBt8M.ARBzD2LVDvMWXOdSsFvDBehPtaR6WVheDUoeM-Fmacw&dib_tag=se&keywords=Low+Noise+Amplifier&qid=1760143315&sr=8-19)
      > 34.95$
    - [Nooelec Lana - Ultra Low-Noise Amplifier (LNA) Module for RF & Software Defined Radio (SDR) with Enclosure & Accessories. Wideband 20MHz-4000MHz Frequency Capability with Bias Tee & USB Power Options](https://www.amazon.com/Nooelec-Lana-Accessories-20MHz-4000MHz-Capability/dp/B07XNLJ9X2/ref=sr_1_3?dib=eyJ2IjoiMSJ9.L5ucGNN3Pdy74YRb2g9q8tZvG8mDnvuTtVVgAgNlx1j55dVgR7DWRaXXZb2k8Py8-mT5AO8e-tPxBRGKUOGsH2loHGIfmgXlRs5FXPDa6TxWlrbYM7-0TCGXYJSkYSdvMqOllgzEIyUvNVaYqk0i-XA1c-qfH8vPfGhOfm9FZH_NlwkXeWLqc7scHCW67s-XTfzqpLvteXbzRenhnK5oPtgqhOwrE2Jq7TmfrLFBt8M.ARBzD2LVDvMWXOdSsFvDBehPtaR6WVheDUoeM-Fmacw&dib_tag=se&keywords=Low+Noise+Amplifier&qid=1760143315&sr=8-3)
      > 34.95$

## Applying to the Pixie Kit:
- low noise amplifiers (LNA's) are not in Pixie Kits
  
- this is because:
  > the overall idea and design of it is very simple so not as many components as a regular radio
  > it only has the bare essentials
  
- instead of a LNA:
  > a combo mixer and power amplifier make up for it
  > also the chip LM386 in one of the versions uses a Low Voltage Audio Power Amplifier could is a somewhat similar comparison or equivalent
