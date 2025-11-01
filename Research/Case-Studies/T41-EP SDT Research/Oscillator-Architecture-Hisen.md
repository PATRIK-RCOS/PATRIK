# T41-EP Software Defined Transceiver
## Technical Design and Construction Note

### 1. System Architecture

The T41-EP is a direct conversion software-defined transceiver operating from 400kHz to 110MHz on receive and covering amateur bands from 630M through 6M on transmit. The design uses a Teensy 4.1 microcontroller (ARM Cortex-M7 at 600MHz) with 7.9MB flash and 1MB RAM to perform all signal processing. The system achieves up to 192kHz spectrum display bandwidth through 24-bit ADC/DAC conversion at 192kSPS.

The architecture eliminates traditional IF stages by down-converting directly to baseband. On receive, RF signals pass through band-specific LC filters before entering the Quadrature Sampling Detector, which produces I and Q baseband signals centered at 48kHz. These signals feed into PCM1808 stereo ADCs on the Teensy Audio Adapter board, where digital filtering, demodulation, and audio processing occur entirely in software. The transmit path reverses this process, with the Teensy generating I/Q signals via PCM5102 DACs that drive the Quadrature Sampling Exciter, which up-converts to RF before amplification and harmonic filtering.

### 2. Quadrature Sampling Detector/Exciter (QSD/QSE) Design

The QSD/QSE board combines both receive and transmit mixing functions using the Tayloe detector topology. The core mixing element is the FST3253 dual 1:4 bus multiplexer, which operates as a four-position electronic switch cycling at the local oscillator frequency. This device must be powered from 5V (absolute maximum 6V) and has a maximum switching frequency of 250MHz according to the datasheet, though practical operation is limited by the quadrature generator circuit.

The quadrature clock generation uses a 74AC74 dual D-type flip-flop configured as a divide-by-4 counter. For the standard T41 V12 design, the Si5351 generates a clock at four times the desired RF frequency, which the 74AC74 divides down while producing two outputs at pins 6 and 9 that are precisely 90 degrees apart. When powered from 3.3V, the 74AC74 has a maximum clock frequency specification of 95MHz, limiting operation on 10 meters. At 28.5MHz operation, the required LO frequency is 4 × (28.5 + 0.048) = 114.192MHz, which exceeds this limit. Operation at 5V extends the maximum frequency to 125MHz, resolving the 10-meter limitation.

Alternative designs like QSD2 use a divide-by-2 configuration that requires only 2× the RF frequency. This approach needs both the clock and its inverse, requiring an additional inverter stage. To maintain quadrature accuracy, a second inverter matches the delay in the signal path. The divide-by-2 configuration enables VHF operation but introduces a low-frequency limit and requires proper reset sequencing to ensure correct quadrature polarity.

The FST3253 switches connect to four matched sampling capacitors, which hold voltage samples at the 0°, 90°, 180°, and 270° phase points of each RF cycle. These capacitors must be matched within 1% for proper image rejection. The original designs specified 100-330pF polyester or C0G/NP0 ceramic capacitors. The capacitors form an RC network with the antenna impedance, creating a natural bandpass filter that tracks the operating frequency.

Instrumentation amplifiers follow the sampling capacitors, configured to amplify the differential signals (0°-180° for I, 90°-270° for Q). The LM4562 operational amplifiers provide low noise and high input impedance to avoid loading the sampling capacitors. DC coupling throughout the I/Q signal chain enables carrier null calibration. The original designs used AC coupling with 1µF capacitors (C9-C12) between the mixing section (5V supply) and the op-amp section (which can operate from 5-35V for increased dynamic range). The V12 design incorporates TX/RX switching using 74HC4053 analog multiplexers or BS170 MOSFETs to route signals between receive and transmit paths.

### 3. Si5351 Local Oscillator System

The Si5351A clock generator provides all local oscillator signals through I²C control from the Teensy 4.1. The device uses a 25MHz crystal reference connected between pins XA and XB, with internal load capacitance set to 10pF through register 183. The crystal oscillator drives two internal PLLs (PLL_A and PLL_B) that multiply the reference to an intermediate frequency between 600-900MHz. For optimal phase noise performance, the PLLs operate between 800-850MHz.

Each output clock derives from a MultiSynth fractional divider that divides the PLL frequency to the desired output. The frequency calculation follows: f_out = f_PLL / (a + b/c), where a is the integer divider (4-2048) and b/c represents the fractional portion using 20-bit numerators and denominators. This provides sub-Hz frequency resolution limited only by the reference crystal accuracy.

For T41 operation, CLK0 generates the receive LO at 4× the operating frequency plus the 48kHz IF offset. CLK1 generates the transmit LO, which may differ from CLK0 during split operation or RIT/XIT use. The Teensy programs the Si5351 via I²C at address 0x60, setting register values to configure the PLLs and MultiSynth dividers. The initialization sequence specifies the crystal load capacitance, reference frequency, and a correction factor determined during calibration.

Frequency calibration compensates for crystal tolerance and load capacitance mismatches. The user tunes to a known-accurate signal (WWV or GPS reference) and adjusts a correction value until the display shows the correct frequency. This correction factor (stored in parts per ten million as an integer) multiplies into all frequency calculations. With standard 25MHz crystals having ±20-50 PPM tolerance, the uncorrected error at 28MHz is approximately ±560-1400Hz. After calibration using an accurate reference, frequency accuracy improves to within ±10Hz across all HF bands.

TCXO (Temperature Compensated Crystal Oscillator) modules can replace the standard crystal for improved stability. These modules connect to the XA pin with XB left floating, providing ±0.5-2.5 PPM accuracy over temperature. The software must specify the TCXO frequency (still 25MHz in most cases) during initialization. The improved stability reduces frequency error at 28MHz to approximately ±14-70Hz without calibration, and to a few Hz after calibration.

The Si5351 output drive strength is programmable from 2-8mA. The T41 uses 2mA drive (approximately 7dBm into 50Ω), which provides sufficient level for the 74AC74 clock input while minimizing harmonic content and power consumption. The Si5351 can enable or disable each output individually through software control, reducing power consumption during transmit by disabling unused receive channels.

### 4. Main Board and DSP Processing

The Main Board houses the Teensy 4.1, the Teensy Audio Adapter board (containing PCM1808 and PCM5102 codecs), display drivers, user interface connections, and power distribution. The Teensy mounts in dual-row female headers, with the Audio Adapter stacking on top via additional headers. This arrangement positions the Audio Adapter's SD card socket toward the right side of the board for accessibility.

The PCM1808 operates in slave mode, receiving bit clock (BCK) and left-right clock (LRCK) from the Teensy's I²S interface. The device performs delta-sigma conversion with 64× oversampling, achieving 99dB SNR and 99dB dynamic range according to the Texas Instruments datasheet. The analog input accepts 3V peak-to-peak single-ended signals, matching the output level from the QSD instrumentation amplifiers. DC blocking capacitors couple the I and Q signals from the QSD to the PCM1808 inputs, though some builders remove these for improved low-frequency response and calibration capability.

The PCM5102 DAC generates the transmit I and Q drive signals. This device also operates in slave mode, deriving its clocks from the Teensy. The DAC includes internal charge pumps for generating negative supply rails and integrated line drivers capable of 3V peak-to-peak output. A simple RC filter at each DAC output removes high-frequency switching artifacts before the signals reach the QSE board.

The Teensy 4.1 processor executes approximately 29,000 lines of C/C++ code derived from Frank Dziock's Convolution SDR project. The code uses the Teensy Audio Library framework with custom DSP routines from the CMSIS DSP library. The I²S audio data streams into circular buffers where FIR and IIR filters implement selectivity. The software supports filter bandwidths from 200Hz to 5kHz through coefficient tables calculated offline and loaded at runtime.

FFT processing for spectrum display uses 1024, 2048, or 4096-point transforms depending on the desired resolution and update rate. The FFT operates on windowed data using Hamming or Hanning windows to reduce spectral leakage. After transformation to frequency domain, the software performs magnitude calculation and dB scaling before sending display data to the TFT controller via SPI.

For SSB generation on transmit, the software applies Hilbert transform filtering to create the quadrature component from the microphone input. The Hilbert transform is implemented as a 65-tap FIR filter with alternating zero and non-zero coefficients. The resulting I and Q signals have a precise 90-degree phase relationship across the audio bandwidth, enabling single-sideband generation through the phasing method. Carrier and sideband suppression exceeds 50dB when the system is properly calibrated for I/Q amplitude and phase balance.

---
## DRAFT SECTIONS BELOW - WORK IN PROGRESS
---

### 5. Power Amplifier Circuit [DRAFT]

The K9HZ 20W power amplifier uses a class AB2 push-pull design with RD16HHF1 RF power MOSFETs as finals. These devices are rated for 16V drain voltage and 30W dissipation, operating conservatively at 12V for extended duty cycle. The finals require approximately 1mW drive from the QSE, with the driver stage providing 30dB of gain.

[TODO: Verify exact bias current specifications from K9HZ schematics]

The bias circuit establishes 20-40mA quiescent current per transistor through the gate voltage divider. Temperature compensation uses a thermistor in the bias network to reduce the temperature coefficient of the gate threshold voltage, preventing thermal runaway. The bias adjustment potentiometer sets the quiescent current, which directly affects linearity and efficiency. Higher bias current improves linearity at the cost of increased idle dissipation, while lower bias risks crossover distortion.

[TODO: Add transformer specifications - core type, turns ratio, primary/secondary turns]


[TODO: Document protection circuit details and threshold values]



### 6. Filter Board Construction [DRAFT - NEEDS VERIFICATION]

The filter board implements both receive bandpass filters and transmit low-pass filters using discrete LC networks. Each amateur band requires its own set of filters, switched by latching relays controlled via I²C expander chips. The board accommodates seven HF bands (80, 40, 20, 17, 15, 12, 10 meters), with provisions for additional filters on 160M and 60M.

[TODO: Get specific component values for each band's filters]
[TODO: Verify relay part numbers and specifications]

[TODO: Document I²C expander circuit and addressing scheme]



### 7. User Interface and Display [DRAFT - INCOMPLETE]

[TODO: This section needs expansion with specific RA8875 register configuration details]



### 8. Power Distribution and Regulation [DRAFT]

[TODO: Need complete schematic review for exact regulator part numbers]

The system requires 12V DC at 3-5A for full power operation. Bulk storage capacitors (1000-4700µF) at the input provide low-impedance supply during transmit RF pulses. Each board contains its own voltage regulators to generate the required 5V and 3.3V rails. The QSD/QSE board must provide clean 5V for the FST3253 and 74AC74, using a low-dropout linear regulator with additional filtering. The main board generates 3.3V for the Teensy and 5V for the Audio Adapter through separate regulators.

[TODO: Measure and document actual current draws per board]



### 9. Calibration and Alignment Procedures [DRAFT - NEEDS TESTING DATA]

Frequency calibration begins by programming the Si5351 to output 10.000000 MHz on CLK2 and measuring the actual frequency with an accurate counter. The error in Hz divided by 10,000,000 gives the correction factor in PPM, which the software stores multiplied by 100 as an integer. This correction applies to all frequency calculations. Alternatively, tuning to WWV or another known signal and adjusting the calibration value until the display shows the correct frequency achieves the same result.


### 10. Construction Considerations [DRAFT]

[TODO: Add photos of construction steps]

Building the T41 requires careful attention to component orientation and soldering technique. The 1206-size SMD components on pre-assembled boards need only through-hole assembly, but builders working with bare PCBs must solder hundreds of SMD parts. Hot air rework for SMD assembly requires controlled temperature (300-350°C air temperature) to avoid damaging components or lifting pads. Solder paste application through stencils ensures consistent results, though hand application with a syringe works for prototyping.

[TODO: Reference QST article on toroid winding techniques]


### 11. Software Architecture [DRAFT - OUTLINE ONLY]

[TODO: This section needs significant expansion with code examples]

The T41 codebase evolved from Convolution SDR, originally written for the Teensy 3.5. Porting to Teensy 4.1 took advantage of the faster processor and larger memory, enabling longer FFT lengths and more complex filtering. The code structure follows a modular organization with separate files for display functions, encoder handling, filters, spectrum processing, and modulation/demodulation.



[TODO: Document filter coefficient generation process]



### 12. Design Trade-offs and Limitations [DRAFT]

[TODO: Add quantitative analysis of trade-offs]

The direct conversion architecture eliminates the need for traditional IF stages and image-reject filters but introduces DC offset and 1/f noise challenges. The 48kHz IF frequency moves the signal away from DC, reducing these problems while keeping the sample rate manageable. The tradeoff is that spectrum display bandwidth is limited to approximately 192kHz, sufficient for most HF operating but less than some commercial SDRs offer.

Component selection balanced performance against cost. The PCM1808/PCM5102 codecs provide 24-bit conversion at 192kHz sample rate for under $5 in quantity, compared to $20-50 for higher-performance converters. The performance difference matters less in an HF transceiver where atmospheric noise and other factors often dominate the system noise floor. Similarly, the RD16HHF1 finals cost significantly less than alternatives like the SD2933 while still delivering adequate power across all HF bands.

The FST3253 switching frequency limitation restricts operation above 50MHz when using the divide-by-4 configuration. Alternative configurations (divide-by-2 or direct Si5351 quadrature output) extend frequency coverage but introduce other compromises. The divide-by-2 approach requires careful attention to clock timing and reset sequencing. Direct Si5351 quadrature output limits low-frequency coverage due to the minimum PLL frequency.

Phase noise from the Si5351 exceeds that of crystal oscillators or high-end synthesizers, though it remains adequate for amateur radio use. Reciprocal mixing (close-in phase noise mixing with strong adjacent signals) rarely causes problems in practice due to the filtering ahead of the mixer. Harmonic content in the Si5351 output requires the bandpass filters to attenuate out-of-band mixing products.

The modular construction with interconnecting cables introduces potential ground loops and RF coupling paths. Careful layout and proper shielding minimize these effects, but some builders experience instability or spurious responses that require additional filtering or grounding improvements. Commercial radios with integrated single-board designs avoid these issues but sacrifice the experimental flexibility that makes the T41 attractive.

### 13. Performance Metrics [DRAFT - NEEDS MEASUREMENT VERIFICATION]

