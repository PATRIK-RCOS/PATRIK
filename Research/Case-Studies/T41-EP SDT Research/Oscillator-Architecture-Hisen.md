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

### 5. Power Amplifier Circuit

The K9HZ 20W power amplifier uses a class AB2 push-pull design with RD16HHF1 RF power MOSFETs as finals. These devices are rated for 16V drain voltage and 30W dissipation, operating conservatively at 12V for extended duty cycle. The finals require approximately 1mW drive from the QSE, with the driver stage providing 30dB of gain.

The bias circuit establishes 20-40mA quiescent current per transistor through the gate voltage divider. Temperature compensation uses a thermistor in the bias network to reduce the temperature coefficient of the gate threshold voltage, preventing thermal runaway. The bias adjustment potentiometer sets the quiescent current, which directly affects linearity and efficiency. Higher bias current improves linearity at the cost of increased idle dissipation, while lower bias risks crossover distortion.

The output matching network uses a broadband transformer wound on a ferrite toroid to match the drain impedance to 50Ω. The transformer ratio and turns count are optimized for operation from 1.8-30MHz, achieving ±2dB power flatness across this range. A parallel RC network across the primary winding dampens parasitic resonances that could cause instability at VHF frequencies.

The amplifier includes short-circuit protection through current sensing in the drain supply. When excessive current flows, the protection circuit removes gate bias, shutting down the transistors. Thermal monitoring can optionally shut down the amplifier if the heatsink temperature exceeds safe limits. The heatsink must dissipate approximately 10W during continuous 20W output operation, requiring forced air cooling or a substantial aluminum extrusion.

Earlier T41 versions used IRF510 MOSFETs, which cost less but show poor efficiency above 15MHz. These devices work adequately on 80-20 meters but deliver only 3-5W on 10 meters due to their increasing gate capacitance and falling gain at HF. The RD16HHF1 maintains better performance across the entire HF spectrum, justifying the higher component cost.

### 6. Filter Board Construction

The filter board implements both receive bandpass filters and transmit low-pass filters using discrete LC networks. Each amateur band requires its own set of filters, switched by latching relays controlled via I²C expander chips. The board accommodates seven HF bands (80, 40, 20, 17, 15, 12, 10 meters), with provisions for additional filters on 160M and 60M.

The bandpass filters use Chebyshev or Butterworth responses with 3-5 poles, providing 40-60dB rejection of out-of-band signals while maintaining 2-3dB insertion loss in-band. Inductors are hand-wound on powdered iron toroids (T50-2, T50-6, T68-2 cores depending on band), with tap positions and turn counts calculated for each specific frequency range. Capacitors use C0G/NP0 dielectric for stability and low loss, with values selected from standard E12 or E24 series to approximate the calculated filter response.

The low-pass filters on the transmit path use 5-7 pole designs with cutoff frequencies just above each band's upper edge. For example, the 20-meter LPF cuts off at approximately 14.5MHz, providing 40dB+ attenuation of the second harmonic at 28MHz and 50dB+ at the third harmonic. Toroid winding for the LPF inductors follows similar procedures as the BPF, though higher inductance values may require larger core sizes or more turns.

Relay selection prioritizes low insertion loss and adequate RF power handling. The relays must handle 20W RF continuously without contact degradation. DPDT relays route both the filter input and output, providing true switching with good isolation between bands. The relay coils draw 50-100mA at 5V, supplied through ULN2803 Darlington arrays driven by I²C port expanders (MCP23017 or similar). The software activates the appropriate relay pair for the selected band while ensuring no two relays select overlapping signal paths.

Proper layout is critical for filter performance. The BPF input and output connections must maintain 50Ω impedance and minimize coupling between adjacent filter sections. Ground planes on both sides of the PCB provide solid RF ground reference, with multiple vias connecting the planes. Component placement keeps input and output sides separated to prevent feedthrough that would degrade filter rejection. Shielding between filter sections may be necessary on the higher bands to achieve specified isolation.

### 7. User Interface and Display

The display interface uses an RA8875 controller driving an 800×480 TFT LCD panel. The RA8875 connects via SPI at 20MHz clock rate, with dedicated chip select and additional control lines for reset and status. The controller includes internal graphics acceleration and character generation, reducing the processing load on the Teensy. Display data transfers use DMA (Direct Memory Access) to minimize CPU intervention during screen updates.

The spectrum and waterfall displays refresh at 10-30 frames per second depending on FFT size and processor loading. The software maintains a circular buffer of FFT magnitude arrays for the waterfall display, scrolling older data downward as new data arrives at the top. Color mapping converts magnitude values to RGB colors using lookup tables, allowing adjustment of the color palette for different viewing preferences or lighting conditions.

Three rotary encoders provide the primary tuning and control inputs. The main tuning encoder connects to interrupt-capable pins, enabling fast frequency changes without polling delays. The encoder produces quadrature output signals (A and B channels) that generate state transitions as the shaft rotates. The software decodes these transitions using a state machine, incrementing or decrementing the frequency based on the direction of rotation. A pushbutton on the encoder shaft switches between different tuning rates (1Hz, 10Hz, 100Hz, 1kHz steps).

The switch matrix uses 18 tactile switches arranged in a 6×3 grid to minimize pin usage. The Teensy scans the matrix by driving one row at a time while reading the column inputs. This scanning occurs at 100-200Hz, fast enough to capture all button presses reliably. Debouncing in software filters mechanical bounce that would otherwise cause multiple triggers from a single press. The switches control band selection, mode, AGC, noise reduction, and menu navigation.

Microphone input enters through a 3.5mm jack with the tip carrying audio and the ring providing PTT (Push To Talk) control. The microphone audio feeds into an analog input on the Teensy Audio Adapter, where the PCM1808 digitizes it at 48kHz sample rate. The software applies speech compression and equalization before modulation processing. PTT detection uses a digital input that switches the radio between receive and transmit modes, activating the appropriate signal paths and enabling the power amplifier.

### 8. Power Distribution and Regulation

The system requires 12V DC at 3-5A for full power operation. Bulk storage capacitors (1000-4700µF) at the input provide low-impedance supply during transmit RF pulses. Each board contains its own voltage regulators to generate the required 5V and 3.3V rails. The QSD/QSE board must provide clean 5V for the FST3253 and 74AC74, using a low-dropout linear regulator with additional filtering. The main board generates 3.3V for the Teensy and 5V for the Audio Adapter through separate regulators.

Ferrite beads in series with power supply lines to sensitive circuits block RF from coupling into the DC rails. Additional bulk and ceramic bypass capacitors placed close to each IC provide local energy storage and high-frequency decoupling. The power amplifier draws pulsed current during transmit peaks, requiring particularly robust supply decoupling to prevent voltage sag that would modulate the RF output.

Ground distribution follows a star configuration where possible, with the main board serving as the central ground reference point. The QSD/QSE and filter boards connect to this central ground through their interconnecting cables. Separate analog and digital ground planes on the main board connect only at a single point near the power input to minimize ground loop noise. RF ground uses the chassis or enclosure, bonded to the circuit ground at one location to avoid multiple ground paths that could create loops.

### 9. Calibration and Alignment Procedures

Frequency calibration begins by programming the Si5351 to output 10.000000 MHz on CLK2 and measuring the actual frequency with an accurate counter. The error in Hz divided by 10,000,000 gives the correction factor in PPM, which the software stores multiplied by 100 as an integer. This correction applies to all frequency calculations. Alternatively, tuning to WWV or another known signal and adjusting the calibration value until the display shows the correct frequency achieves the same result.

I/Q balance calibration compensates for amplitude and phase mismatches in the QSD and analog signal path. The procedure connects a signal generator to the antenna input and adjusts software parameters until the image frequency null maximizes. The software can apply independent corrections to the I and Q channel gains (typically within ±5%) and phase offset (typically within ±5 degrees). Proper balance achieves >50dB image rejection, making the unwanted sideband effectively inaudible during SSB reception.

Transmitter carrier null calibration requires measuring the carrier leakage with a spectrum analyzer or receiver while transmitting without audio input. Software adjustments trim DC offsets in the I and Q DAC outputs until the carrier reaches minimum amplitude, achieving >50dB carrier suppression. This calibration is temperature-sensitive and may require occasional adjustment.

Power output calibration verifies that the software power control produces the indicated power level. Connecting a dummy load and power meter allows measuring actual output versus the software setting. The firmware contains a calibration table that maps power control values to actual output power for each band, compensating for variations in amplifier gain across frequency.

S-meter calibration uses a calibrated signal generator at the antenna input. Known signal levels (e.g., -73dBm = S9, -121dBm = S1) are applied while adjusting the S-meter scaling factors in software. The logarithmic scaling maps ADC values to S-units, with corrections for each band to account for different bandpass filter insertion losses.

### 10. Construction Considerations

Building the T41 requires careful attention to component orientation and soldering technique. The 1206-size SMD components on pre-assembled boards need only through-hole assembly, but builders working with bare PCBs must solder hundreds of SMD parts. Hot air rework for SMD assembly requires controlled temperature (300-350°C air temperature) to avoid damaging components or lifting pads. Solder paste application through stencils ensures consistent results, though hand application with a syringe works for prototyping.

Toroid winding consumes 8-12 hours of construction time. The enameled wire must be wound tightly and evenly around the core, counting turns carefully. Scraping the enamel from wire ends before tinning ensures good solder joints. Core sizes and permeability types follow published designs, with T50-2 (red) cores common for HF applications. The winding direction matters for phased filters, where coupling between inductors affects the response.

Interconnect cables between boards must maintain signal integrity. The I/Q audio signals between the main board and QSD/QSE use shielded stereo audio cable to prevent RF pickup and crosstalk. Short coaxial cables (semi-rigid or flexible RG-316) connect the RF signal paths, keeping impedance controlled and minimizing loss. IDC ribbon cables work for digital control signals and power distribution, with proper attention to pin assignments during assembly.

Testing progresses board by board, verifying each module's functionality before integration. The main board should boot and initialize the display before adding other modules. The QSD receives first, checking for proper LO frequency and audio output with a known signal. Only after successful reception should transmit testing begin, initially at low power into a dummy load to prevent damage from mistuning or improper bias settings.

Enclosure selection affects thermal management and RF shielding. Metal enclosures provide superior RF containment compared to plastic, reducing spurious emissions and susceptibility to external interference. Ventilation holes or forced-air cooling handle the 10W heat dissipation during full-power transmission. Proper panel layout minimizes cable lengths and allows comfortable operation of controls while viewing the display.

### 11. Software Architecture

The T41 codebase evolved from Convolution SDR, originally written for the Teensy 3.5. Porting to Teensy 4.1 took advantage of the faster processor and larger memory, enabling longer FFT lengths and more complex filtering. The code structure follows a modular organization with separate files for display functions, encoder handling, filters, spectrum processing, and modulation/demodulation.

The main loop executes at approximately 1kHz, handling user input, display updates, and housekeeping tasks. DSP processing occurs in interrupt service routines triggered by the I²S interface when new audio samples arrive. The audio library uses DMA to transfer samples between peripherals and memory without CPU intervention, leaving the processor free for DSP calculations.

FIR filters implement most of the selectivity and equalization functions. The filter coefficients load from tables in flash memory, calculated using external tools (MATLAB or SciPy) and converted to fixed-point or floating-point format. The Teensy 4.1's floating-point unit accelerates filter calculations, though fixed-point arithmetic may offer better performance for longer filters. The CMSIS DSP library provides optimized implementations of convolution, correlation, and transform functions that exploit the ARM processor's single-instruction multiple-data capabilities.

AGC (Automatic Gain Control) adjusts receive gain to maintain constant audio level despite varying signal strength. The algorithm measures signal amplitude over 10-100ms windows, compares to a target level, and adjusts the gain multiplier accordingly. Attack time (how quickly gain decreases for strong signals) is faster than decay time (how quickly gain recovers after strong signals pass), preventing the AGC from "pumping" on SSB speech. Multiple AGC modes (fast, medium, slow, off) provide different time constants for different operating conditions.

Noise reduction algorithms operate in the frequency domain, identifying spectral bins containing primarily noise and applying attenuation. The LMS (Least Mean Squares) adaptive filter adjusts its coefficients to minimize the difference between the desired signal and the filter output, effectively learning the noise characteristics and subtracting them. The notch filter implements a narrow rejection at a specific frequency, useful for eliminating carrier tones or heterodyne interference.

### 12. Design Trade-offs and Limitations

The direct conversion architecture eliminates the need for traditional IF stages and image-reject filters but introduces DC offset and 1/f noise challenges. The 48kHz IF frequency moves the signal away from DC, reducing these problems while keeping the sample rate manageable. The tradeoff is that spectrum display bandwidth is limited to approximately 192kHz, sufficient for most HF operating but less than some commercial SDRs offer.

Component selection balanced performance against cost. The PCM1808/PCM5102 codecs provide 24-bit conversion at 192kHz sample rate for under $5 in quantity, compared to $20-50 for higher-performance converters. The performance difference matters less in an HF transceiver where atmospheric noise and other factors often dominate the system noise floor. Similarly, the RD16HHF1 finals cost significantly less than alternatives like the SD2933 while still delivering adequate power across all HF bands.

The FST3253 switching frequency limitation restricts operation above 50MHz when using the divide-by-4 configuration. Alternative configurations (divide-by-2 or direct Si5351 quadrature output) extend frequency coverage but introduce other compromises. The divide-by-2 approach requires careful attention to clock timing and reset sequencing. Direct Si5351 quadrature output limits low-frequency coverage due to the minimum PLL frequency.

Phase noise from the Si5351 exceeds that of crystal oscillators or high-end synthesizers, though it remains adequate for amateur radio use. Reciprocal mixing (close-in phase noise mixing with strong adjacent signals) rarely causes problems in practice due to the filtering ahead of the mixer. Harmonic content in the Si5351 output requires the bandpass filters to attenuate out-of-band mixing products.

The modular construction with interconnecting cables introduces potential ground loops and RF coupling paths. Careful layout and proper shielding minimize these effects, but some builders experience instability or spurious responses that require additional filtering or grounding improvements. Commercial radios with integrated single-board designs avoid these issues but sacrifice the experimental flexibility that makes the T41 attractive.

### 13. Performance Metrics

Receiver sensitivity measurements show minimum discernible signal (MDS) of approximately -130dBm in 500Hz bandwidth, limited primarily by the QSD noise figure and subsequent amplification. The noise figure of the FST3253 itself is under 10dB, with proper biasing and matching. The instrumentation amplifiers contribute 3-5dB additional noise. AGC reduces the effective noise floor by bringing weak signals up in level, making signals down to -135dBm audible in good conditions.

Dynamic range measurements depend on the measurement technique. The ADC provides 110dB theoretical dynamic range based on its 24-bit resolution, but practical limitations reduce this to approximately 90dB. Third-order intercept point (IP3) measurements on the QSD alone show values around +10 to +15dBm, which translates to approximately 90dB dynamic range for 1kHz spacing between interfering signals. Stronger signals or closer spacing reduce the dynamic range due to intermodulation products.

Transmitter spurious emissions meet FCC Part 97 requirements with properly functioning harmonic filters. Second harmonic suppression exceeds 40dB and third harmonic exceeds 45dB. Close-in spurious products from the Si5351 appear at -40 to -50dBc and require the bandpass filters for adequate suppression. IMD products in the power amplifier depend on bias adjustment, achieving -30dBc with proper setting but degrading if bias is too low.

Frequency stability with a standard 25MHz crystal varies ±50 PPM over temperature, requiring occasional frequency adjustments during long operating sessions. TCXO upgrade reduces drift to ±1-2 PPM, making frequency adjustments unnecessary except for the initial calibration. Even with standard crystals, post-calibration accuracy reaches ±10Hz, adequate for SSB and CW but marginal for digital modes requiring tight frequency control.
