# Low Noise Amplifier Chosen for Purchase
- [Lana HF v2 - Ultra Low-Noise LF, MF & HF Amplifier (LNA) Module for RF & Software Defined Radio (SDR). Wideband 50kHz-150MHz Frequency Capability with Bias Tee & USB Power Options](https://www.amazon.com/Lana-HF-Low-Noise-50kHz-150MHz-Capability/dp/B0BBSPDJBG/ref=sr_1_19?dib=eyJ2IjoiMSJ9.L5ucGNN3Pdy74YRb2g9q8tZvG8mDnvuTtVVgAgNlx1j55dVgR7DWRaXXZb2k8Py8-mT5AO8e-tPxBRGKUOGsH2loHGIfmgXlRs5FXPDa6TxWlrbYM7-0TCGXYJSkYSdvMqOllgzEIyUvNVaYqk0i-XA1c-qfH8vPfGhOfm9FZH_NlwkXeWLqc7scHCW67s-XTfzqpLvteXbzRenhnK5oPtgqhOwrE2Jq7TmfrLFBt8M.ARBzD2LVDvMWXOdSsFvDBehPtaR6WVheDUoeM-Fmacw&dib_tag=se&keywords=Low+Noise+Amplifier&qid=1760143315&sr=8-19)

## The Previously Researched Final Two Options 
1)[Lana HF v2 - Ultra Low-Noise LF, MF & HF Amplifier (LNA) Module for RF & Software Defined Radio (SDR). Wideband 50kHz-150MHz Frequency Capability with Bias Tee & USB Power Options](https://www.amazon.com/Lana-HF-Low-Noise-50kHz-150MHz-Capability/dp/B0BBSPDJBG/ref=sr_1_19?dib=eyJ2IjoiMSJ9.L5ucGNN3Pdy74YRb2g9q8tZvG8mDnvuTtVVgAgNlx1j55dVgR7DWRaXXZb2k8Py8-mT5AO8e-tPxBRGKUOGsH2loHGIfmgXlRs5FXPDa6TxWlrbYM7-0TCGXYJSkYSdvMqOllgzEIyUvNVaYqk0i-XA1c-qfH8vPfGhOfm9FZH_NlwkXeWLqc7scHCW67s-XTfzqpLvteXbzRenhnK5oPtgqhOwrE2Jq7TmfrLFBt8M.ARBzD2LVDvMWXOdSsFvDBehPtaR6WVheDUoeM-Fmacw&dib_tag=se&keywords=Low+Noise+Amplifier&qid=1760143315&sr=8-19)

<ul>
    <li>Price: $34.95</li>
    <li>Connectors: SMA, USB</li>
    <li>Features:
    <ul>
        <li>High gain = +20dB</li>
        <li>Low Noise Figure = 2.3-2.7dB
        <ul>
            <li>usually 2.5dB</li>
        </ul>
        </li>
        <li>Voltage = 3.3-5.5V
        <ul>
            <li>usually 5V</li>
        </ul>
        </li>
        <li>Current Use = 25mA</li>
        <li>High Linearity Module</li>
    </li>
    </ul>
</ul>

     
2)[Nooelec Lana - Ultra Low-Noise Amplifier (LNA) Module for RF & Software Defined Radio (SDR) with Enclosure & Accessories. Wideband 20MHz-4000MHz Frequency Capability with Bias Tee & USB Power Options](https://www.amazon.com/Nooelec-Lana-Accessories-20MHz-4000MHz-Capability/dp/B07XNLJ9X2/ref=sr_1_3?dib=eyJ2IjoiMSJ9.L5ucGNN3Pdy74YRb2g9q8tZvG8mDnvuTtVVgAgNlx1j55dVgR7DWRaXXZb2k8Py8-mT5AO8e-tPxBRGKUOGsH2loHGIfmgXlRs5FXPDa6TxWlrbYM7-0TCGXYJSkYSdvMqOllgzEIyUvNVaYqk0i-XA1c-qfH8vPfGhOfm9FZH_NlwkXeWLqc7scHCW67s-XTfzqpLvteXbzRenhnK5oPtgqhOwrE2Jq7TmfrLFBt8M.ARBzD2LVDvMWXOdSsFvDBehPtaR6WVheDUoeM-Fmacw&dib_tag=se&keywords=Low+Noise+Amplifier&qid=1760143315&sr=8-3)

<ul>
    <li>Price: $34.95</li>
    <li>Connectors: SMA, USB</li>
    <li>Features:
    <ul>
        <li>High gain = +20dB</li>
        ^ this is at specifically 1000 MHz
        <li>Low Noise Figure = 0.8-1dB
        <ul>
            <li>usually 0.9dB</li>
        </ul>
        </li>
        <li>Voltage = 3.3-5.5V
        <ul>
            <li>usually 5V</li>
        </ul>
        </li>
        <li>Current Use = 85mA</li>
        <li>High Linearity Module</li>
    </li>
    </ul>
</ul>


  
## Reasoning 
### Specifications
<ul>
    <li>This Project is for a 10 Meter HF Radio
    <ul>
        <li>Functions include both a reciever and transmitter</li>
        <li>Mobile Operations so portable and not too high power consumption is favorable
            <ul>
        <li>the current draw is lower for the LNA for 50kHZ-150MHz at 25mA in comparison to 85mA of the 20MHz-4000MHz LNA.</li>
        <li>this makes sense as typically the current draw would increase as the frequency range that the LNA serves increases and also lowers the noise figure.</li>
        <li>while this is shown in this example as in most cases, it is most likely a better choice in this project to choose the LNA with the lower current draw to also reduce or not experiance high power consumption in portable operations.</li>
            </ul>
        </li>
        <li>10 Meters has frequency range of 28.000 MHz - 29.700 MHz</li>
    </ul>
        Both options include 28-29.700MHz within their frequency ranges of 50kHz-150MHz and 20MHz-4000MHz
But since the frequency is 28-29.700 primarily with no need for the extra range of the LNA that goes to 4000MHz
So it makes more sense for this project to select the LNA for 50kHz-150MHz rather than the 20MHz-4000MHz.
    </li>
</ul>

<ul>
    <li>High Linearity Module
<ul>
 <li>This means that there is a linear relationship</li>
 <li>This feature allows for a signal to be ouputted with minimal to no distortion while also amplifying it.</li>
 <li>For the 10 meter HF radio this is important for recieving and transmitting signals.
     <ul>
         <li>Decrease distortion and/or interference</li>
         <li>Locate signals that are obstructed or weak</li>
     </ul>
 </li>
    <li>This part is included in both options for the LNA so not much here to favor one over the other</li>
</ul>
    </li>
    <li>The Voltage or supply is the same range of 3.3-5.5V with a typical of 5V so not much to favor one over the other in the section.</li>
    <li>The Noise Figure is detailed as higher in the 50kHz-150MHz LNA in comparison to the 20MHz-4000MHz LNA.</li>
The 50kHz-150MHz LNA's noise figure is 2.5dB which is higher than 0.9dB of the other LNA.
The noise figure of a low 0.9dB is only really at 1000MHz in the range of 20MHz-4000MHz though still does offer a low noise of 0.8 to a high of 1 in other parts. 
So, in this case it may be beneficial to choose the lower noise figure LNA, but as detailed above, we are favoring the smaller frequency range LNA as our operations will be wihtin them still and it is not really necessary to get the LNA with the much larger range.
</ul>


# Conclusion
>The selected LNA is the: Lana HF v2 - Ultra Low-Noise LF, MF & HF Amplifier (LNA) Module for RF & Software Defined Radio (SDR). Wideband 50kHz-150MHz Frequency Capability with Bias Tee & USB Power Options.
>Overall the features/specifications for this product were pretty similar with the other option though a few factors resulted in the choosing of this over the 20MHz-4000MHz LNA. 
>The factors were the current draw and as seen highlighted above several times, the frequency range. 
>This, the frequency range, was a recurring discussion point in group meetings and ultimately led to the final decision of choosing the LNA with the smaller range that still included the 10 meter freqencies. 
>The larger range LNA could prove useful in other project scenarios though in our case it is not needed.
>Potentially it could disadvantage the operations a bit in current draw at 85mA.
>We are aiming for portable have a higher current draw would drain the power supply or batteries quicker.
>This would be in opposition to the idea of the radio being a small easy to carry radio if the operator was required to bring extra batteries or power to use it.
