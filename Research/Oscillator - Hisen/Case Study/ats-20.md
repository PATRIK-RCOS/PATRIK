# ATS-20/ATS-20+ SDR Receiver - Technical Notes

## Overview

The ATS-20 is a compact, portable **receiver-only** radio based on the Silicon Labs Si4732-A10 single-chip DSP receiver. Despite its low cost (~$25-35), it provides impressive multi-band reception capabilities, making it popular for shortwave listening, travel, and emergency preparedness.

**Key Specifications:**
- **Type:** Receive-only portable radio
- **Frequency Coverage:**
  - FM: 64-108 MHz
  - AM (MW): 520-1710 kHz
  - SW: 2.3-26.1 MHz (includes all amateur bands)
  - LW: 153-279 kHz
- **Modes:** AM, FM, SSB (LSB/USB with patch)
- **Core IC:** Si4732-A10 (Silicon Labs/Skyworks)
- **Controller:** Arduino Nano or compatible
- **Display:** 0.96" OLED
- **Battery:** 800 mAh Li-ion (3.6V)
- **Size:** ~89 × 107 × 41 mm
- **Architecture:** Digital Low-IF with integrated DSP

---

## System Architecture Overview

### Block Diagram

```
ANTENNA
   |
   +--[FM Input] --> FMI Pin (64-108 MHz)
   |
   +--[AM/SW/LW] --> AMI Pin (153 kHz - 26.1 MHz)
                       |
                  Si4732-A10 Single Chip
                       |
       +---------------+---------------+
       |               |               |
   RF Front-End    DSP Core      Audio Output
   (LNA, Mixer)  (Filters, Demod) (DAC, Amp)
       |               |               |
   Integrated VCO  Digital Filters  LOUT/ROUT
       |               |               |
       +----------- I2C Control -------+
                       |
                  Arduino Nano
                  (ATmega328P)
                       |
              +--------+--------+
              |                 |
          OLED Display      Controls
          (I2C/SPI)       (Encoder, Buttons)
```

**Key Design Feature:** 
Almost the entire radio receiver function is contained within the Si4732-A10 chip. The Arduino simply provides user interface and control via I2C commands.

---

## Si4732-A10 Single-Chip Receiver

### Chip Overview

The Si4732-A10 is a highly integrated CMOS AM/FM/SW/LW receiver that includes:
- Dual RF front-ends (FM and AM/SW/LW)
- Integrated frequency synthesizer with VCO
- Digital low-IF receiver architecture
- DSP for filtering and demodulation
- Audio DAC and amplifier drivers
- RDS/RBDS decoder
- Control interface (I2C)

**Manufacturing Process:** CMOS
**Package:** 16-pin SOIC
**Power Supply:** 2.7-3.6V
**Current Consumption:**
- FM receive: ~30 mA
- AM receive: ~25 mA
- Powerdown: <2 µA

---

## Reference Oscillator and Clock Generation (Critical Section)

### Crystal Oscillator Reference

**Crystal Requirements:**
- **Frequency:** 32.768 kHz (typical) or external clock
- **Type:** Parallel resonant crystal
- **Load Capacitance:** 12.5 pF typical
- **Accuracy Required:** ±20 ppm for proper operation
- **ESR:** <50 kΩ maximum

**Crystal Oscillator Circuit:**
```
    RCLK Pin
        |
    [Crystal] -- 32.768 kHz
        |
    Load Caps (internal to Si4732)
        |
       GND
```

**Alternative: External Clock:**
- Can accept external reference clock
- Frequency: 31.13 kHz to 40 MHz
- Logic level: CMOS compatible
- Some ATS-20 units may use Arduino's crystal as reference

**Why 32.768 kHz?**
- Standard "watch crystal" frequency
- Easy to divide to 1 Hz (2^15 = 32,768)
- Low power consumption
- Inexpensive and readily available
- Provides time base for digital processing

**Internal Clock Multiplier:**
- Si4732 internally multiplies 32.768 kHz to higher frequencies
- Generates all needed internal clocking
- No external high-frequency crystal needed

---

## Integrated Frequency Synthesizer

### VCO and PLL Architecture

The Si4732-A10 contains an **integrated Voltage-Controlled Oscillator (VCO)** and **Phase-Locked Loop (PLL)** system.

**Block Diagram:**
```
32.768 kHz Reference
        |
    Reference Divider
        |
    Phase Detector
        |
    Loop Filter (internal)
        |
    VCO (integrated)
        |
    +---+--- to Mixers
        |
    Programmable Divider
        |
    (feedback to Phase Detector)
```

**Key Features:**
- **Integrated VCO:** No external tank circuit needed
- **Digital Tuning:** Frequency set via I2C commands
- **Frequency Resolution:** 1 kHz (AM/SW/LW), 10 kHz or selectable (FM)
- **Tuning Range:**
  - FM VCO: ~128-216 MHz (2× final frequency)
  - AM/SW/LW VCO: ~1-52 MHz range
- **Lock Time:** <50 ms typical
- **Phase Noise:** Optimized for broadcast reception

---

### Frequency Synthesis Examples

**FM Band (88.5 MHz example):**

The Si4732 uses a **Low-IF architecture** for FM:

1. **RF Input:** 88.5 MHz
2. **LO Frequency:** ~87.725 MHz (approximately)
3. **IF Output:** ~775 kHz (Low-IF, not zero-IF)
4. **Digital Processing:** Converts IF to baseband digitally

**PLL Configuration:**
```
VCO = N × f_ref

For FM LO at 87.725 MHz:
N = 87,725,000 / 32,768 ≈ 2,677

Actually uses higher reference after internal multiplication
```

**SW Band (7.050 MHz - 40m ham band):**

1. **RF Input:** 7.050 MHz
2. **LO Frequency:** ~6.595 MHz
3. **IF Output:** ~455 kHz (standard AM IF)
4. **Digital Processing:** Demodulates at IF

**Advantages of Low-IF:**
- Image rejection easier than zero-IF
- No DC offset problems
- Better I/Q balance than analog
- Digital filtering provides selectivity

---

### Digital Low-IF Architecture

**Why "Low-IF" instead of traditional superheterodyne?**

Traditional superhet uses multiple conversions:
```
RF → 1st Mixer → 1st IF (high) → 2nd Mixer → 2nd IF (455 kHz) → Demod
```

Si4732 Low-IF uses:
```
RF → Mixer → Low IF (~455 kHz to 1 MHz) → ADC → Digital Demod
```

**Process:**

1. **RF Signal** enters chip (FM or AM/SW/LW)
2. **LNA (Low Noise Amplifier)** provides gain (~20 dB)
3. **Mixer** downconverts RF to Low-IF
   - Uses synthesized LO from integrated PLL/VCO
4. **Low-IF** signal is amplified
5. **ADC** digitizes the IF signal
6. **DSP** performs:
   - Digital filtering (replaces crystal/ceramic filters)
   - Demodulation (AM, FM, or SSB)
   - AGC
   - Audio processing

**Advantages:**
- Single conversion (simpler)
- No alignment needed
- Digital filters = perfect response
- Firmware upgradeable
- Lower cost than traditional design

---

## RF Front-End

### FM Front-End (64-108 MHz)

**Input:** FMI pin

**Signal Path:**
```
FM Antenna --> Matching --> LNA --> Bandpass Filter --> Mixer
                                                          ↑
                                                        LO (VCO)
```

**Components (all integrated):**
- **LNA:** Low noise amplifier
  - Noise Figure: ~3 dB
  - Gain: ~20 dB
  - Input impedance: 50Ω
- **Bandpass Filter:** 
  - Rejects out-of-band signals
  - Reduces images
- **Mixer:** 
  - Converts FM RF to Low-IF
  - Type: Gilbert cell mixer

**Image Frequency:**
- For 88.5 MHz with LO at 87.725 MHz, IF at 775 kHz
- Image would be at: 87.725 - 0.775 = 86.950 MHz
- ~1.5 MHz separation, filtered by front-end

### AM/SW/LW Front-End (153 kHz - 26.1 MHz)

**Input:** AMI pin

**Signal Path:**
```
Loop/Whip Antenna --> Matching --> LNA --> AGC --> Mixer --> IF
                                                      ↑
                                                    LO (VCO)
```

**LNA Characteristics:**
- Gain: ~30 dB
- Noise Figure: ~10-15 dB (typical for AM)
- Wide dynamic range
- Input impedance: High (for ferrite loop antennas)

**AGC (Automatic Gain Control):**
- Fast attack: ~5 ms
- Slow decay: ~100 ms
- Prevents overload from strong signals
- Digital AGC processing in DSP

---

## Analog-to-Digital Conversion

### ADC Subsystem

**ADC Specifications:**
- **Resolution:** 16-bit typical
- **Sample Rate:** Variable depending on mode
  - FM: ~256 kHz typical
  - AM: ~64 kHz typical
- **Type:** Sigma-Delta (ΔΣ)
- **Input:** Low-IF signal (~455 kHz - 1 MHz)
- **Dynamic Range:** ~80-90 dB

**Sigma-Delta ADC Advantages:**
- High resolution
- Built-in anti-aliasing (oversampling)
- Good for slowly varying signals (audio)
- CMOS friendly (on-chip integration)

**Signal Chain:**
```
Low-IF (~455 kHz) → Anti-alias Filter → ADC → Digital IF Processing
```

---

## Digital Signal Processing (DSP Core)

### DSP Functions

The Si4732's internal DSP performs:

**1. Digital Down-Conversion (DDC):**
- Converts digitized IF to baseband
- Uses Numerical Controlled Oscillator (NCO)
- Provides I/Q (quadrature) signals
- Eliminates analog I/Q imbalance issues

**2. Channel Filtering:**
- **FM Mode:**
  - Bandwidth: ~200 kHz typical
  - Stereo MPX decoding
  - De-emphasis: 50µs or 75µs (programmable)
- **AM Mode:**
  - Seven selectable bandwidths: 6, 4, 3, 2, 1, 1.8, 2.5 kHz
  - Software selectable via I2C
  - No hardware changes needed
- **SSB Mode (with patch):**
  - BFO (Beat Frequency Oscillator) in software
  - USB/LSB selection
  - Typical bandwidth: 2.2 kHz

**3. Demodulation:**
- **FM:** Quadrature detection (digital)
- **AM:** Envelope detection (digital)
- **SSB:** Product detector with BFO

**4. Audio Processing:**
- De-emphasis (FM)
- Bass/treble EQ
- Volume control (digital)
- Soft mute
- Noise blanking

**5. Advanced Features:**
- **RDS/RBDS Decoder:** Decodes station information
- **AFC (Automatic Frequency Control):** Keeps station centered
- **Seek/Scan:** Automatic station search
- **RSSI (Received Signal Strength Indicator):** Signal level measurement
- **SNR Measurement:** Signal-to-noise ratio calculation

---

## SSB Reception (Firmware Patch)

### SSB Capability via Firmware

The Si4732-A10 variant includes **SSB (Single Sideband) capability** through firmware patches.

**SSB Implementation:**

**1. BFO (Beat Frequency Oscillator):**
- Implemented digitally in DSP
- Offset frequency: Adjustable ±10 kHz
- Resolution: ~1 Hz steps
- Replaces missing carrier in SSB signal

**2. Product Detection:**
```
SSB Signal (no carrier) × BFO → Audio
```

**3. Filter Selection:**
- Narrow bandwidth filters (2.2 kHz typical)
- Steep skirts for adjacent channel rejection
- Software filter coefficients

**Loading SSB Patch:**
- Arduino sends patch data via I2C
- Si4732 loads patch into internal RAM
- Patch enables SSB demodulation mode
- Can switch between AM/FM/SSB modes

**BFO Control in ATS-20:**
- Fine-tuning knob adjusts BFO
- Allows zero-beating SSB signals
- Essential for making speech intelligible
- Adjustable in Hz steps

---

## Arduino Nano Controller

### Microcontroller Role

The Arduino Nano (ATmega328P) serves as:

**1. User Interface Controller:**
- Reads rotary encoder for tuning
- Reads push buttons for mode/band selection
- Updates OLED display
- Manages menu system

**2. I2C Master:**
- Sends commands to Si4732
- Reads status from Si4732
- Controls frequency, mode, bandwidth, etc.
- Loads SSB firmware patch

**3. Data Processing:**
- Converts encoder pulses to frequency changes
- Manages band memory
- Stores user preferences in EEPROM
- Formats display data

**Typical I2C Commands:**
```
// Set FM frequency to 88.5 MHz
i2c.send(SI4732_ADDR, FM_TUNE_FREQ, 0, 8850);

// Set AM bandwidth to 2 kHz
i2c.send(SI4732_ADDR, AM_SET_BW, 2);

// Read RSSI
rssi = i2c.read(SI4732_ADDR, GET_STATUS);
```

---

## Display and User Interface

### OLED Display (0.96" 128×64)

**Display Information:**
- **Frequency:** Large digits, main display
- **Mode:** AM/FM/LSB/USB
- **Band:** 80m, 40m, 30m, 20m, etc.
- **Signal Strength:** S-meter or RSSI bar
- **Bandwidth:** Selected filter
- **Additional:** Battery, RDS text, etc.

**Communication:**
- **Interface:** I2C (sometimes SPI)
- **Update Rate:** ~10 Hz typical
- **Refresh:** Full screen or partial updates

### Controls

**Typical Control Layout:**
- **Main Encoder:** Frequency tuning
- **Mode Button:** Cycle through AM/FM/LSB/USB
- **Band Button:** Select preset bands
- **BW Button:** Change bandwidth (AM mode)
- **Volume Knob:** (may be encoder or pot)
- **Power Switch:** On/off

**Frequency Steps:**
- FM: 100 kHz or 50 kHz
- AM: 1 kHz or 5 kHz or 10 kHz
- SW: 1 kHz typical
- SSB: Fine tuning in Hz when BFO adjusted

---

## Audio Output

### DAC and Audio Amplifier

**Si4732 Audio Outputs:**
- **LOUT:** Left audio output
- **ROUT:** Right audio output
- **Format:** Analog audio (DAC output)
- **Level:** Line level (~1V peak-to-peak)
- **Impedance:** Low impedance output

**External Audio Amplifier:**
- **ATS-20 uses:** Simple audio amplifier IC
- **Typical IC:** LM386, PAM8403, or similar
- **Power:** 1.5W into 8Ω speaker (ATS-20+)
- **Headphone Jack:** 3.5mm stereo
- **Speaker:** Internal 8Ω 1.5W

**Audio Path:**
```
Si4732 LOUT/ROUT → Volume Control → Audio Amp → Speaker/Headphones
```

---

## Power Supply

### Battery and Regulation

**Battery:**
- **Type:** Single-cell Li-ion (3.6V/3.7V nominal)
- **Capacity:** 800 mAh typical
- **Runtime:** 
  - Receive (low volume): 15-20 hours
  - Receive (medium volume): 10-15 hours
  - Display brightness affects consumption

**Voltage Regulation:**

```
3.6V Li-ion Battery
        |
    +---+---+
    |       |
  5V Boost   3.3V LDO
    |           |
Arduino     Si4732 + OLED
+ Audio Amp   (Low power)
```

**Power Consumption:**
- **Si4732:** 25-30 mA (receiving)
- **Arduino Nano:** 15-20 mA
- **OLED Display:** 10-20 mA (depends on content)
- **Audio Amp:** 5-50 mA (depends on volume)
- **Total:** ~60-120 mA typical

**Charging:**
- **Interface:** USB Type-C or Micro-USB
- **Charger IC:** TP4056 or similar Li-ion charger
- **Charge Current:** 500 mA typical
- **Charge Time:** ~2 hours from empty

---

## Frequency Stability and Accuracy

### Crystal Tolerance Effects

**32.768 kHz Crystal Tolerance:**
- **Typical:** ±20 ppm
- **At 7 MHz:** ±140 Hz error
- **At 28 MHz:** ±560 Hz error

**Impact on Reception:**
- **FM:** No noticeable effect (bandwidth ~200 kHz)
- **AM:** Slight detuning (bandwidth 6-10 kHz)
- **SSB:** Significant! Speech becomes unintelligible beyond ~100 Hz error
- **Digital Modes:** Can cause decode failures

**Solutions:**
1. **Better crystal:** Use ±10 ppm or TCXO
2. **Calibration:** Tune to known frequency (WWV, time signal)
3. **BFO adjustment:** Manually correct in SSB mode
4. **External reference:** Some mods use GPS or TCXO

**Temperature Effects:**
- Crystal frequency drifts with temperature
- Typical: ±20-50 Hz over 0-40°C range
- Warmup drift: First 5-10 minutes
- Noticeable in SSB and digital modes

---

## Performance Characteristics

### Receiver Sensitivity

**Typical Sensitivities (10 dB S/N):**
- **FM:** 2-5 µV
- **AM:** 20-50 µV
- **SW:** 1-5 µV (varies by band)
- **SSB:** 0.5-2 µV

**Factors Limiting Sensitivity:**
- Noise figure of LNA
- Internal processing noise
- Antenna efficiency (built-in telescopic)
- Environmental noise floor

### Selectivity

**AM Bandwidth Options:**
- 6 kHz (wide, good for strong signals)
- 4 kHz (medium)
- 3 kHz (narrow)
- 2 kHz (very narrow, reduces interference)
- 1 kHz (extremely narrow, CW)
- Additional: 1.8 kHz, 2.5 kHz

**Filter Shape:**
- DSP filters provide very steep skirts
- Shape factor: <2:1 (-6dB / -60dB)
- Much better than ceramic/crystal filters
- No ringing or overshoot

**Adjacent Channel Rejection:**
- Excellent due to DSP filtering
- >60 dB typical for narrow bandwidths
- Limited mainly by RF front-end, not IF

### Dynamic Range

**Blocking Dynamic Range:** ~70-80 dB
- Limited by:
  - RF front-end overload
  - ADC clipping
  - Insufficient AGC range

**Third-Order Intercept Point (IP3):**
- ~-10 dBm typical
- Moderate performance (not a communications receiver)
- Adequate for most shortwave listening

**Intermodulation:**
- Can occur with very strong signals
- Two strong signals can create "phantom" signals
- Use attenuator or better antenna with filtering

---

## Common Modifications and Improvements

### Firmware Upgrades

**PU2CLR Library (Ricardo Lima Caratti):**
- Open-source Arduino library for Si47xx chips
- Adds features:
  - Better SSB support
  - More bandwidth options
  - Improved user interface
  - Spectrum display
  - Step attenuator control

**Custom Firmware:**
- Many hobbyists develop custom firmware
- Can add:
  - Frequency memories
  - Scanning functions
  - Custom band plans
  - S-meter improvements

### Hardware Modifications

**1. Better Antenna:**
- Replace telescopic with random wire
- Add external antenna jack (SMA)
- Use active antenna for better sensitivity

**2. External Reference:**
- Replace 32.768 kHz crystal with TCXO
- Add GPS-disciplined oscillator
- Dramatically improves frequency accuracy for SSB

**3. Audio Improvements:**
- Larger speaker (better bass response)
- Better headphone amplifier
- External audio filter

**4. Display Upgrade:**
- Larger OLED (1.3" or 1.5")
- Color TFT display
- More information displayed

**5. Better Enclosure:**
- Aluminum case (better shielding)
- More robust construction
- Improved ergonomics

---

## Comparison with Other Receivers

### vs. Traditional Analog Receivers

**ATS-20 Advantages:**
- Extremely compact
- No alignment needed
- DSP filtering (perfect filters)
- Low cost
- Firmware upgradeable
- Digital display with spectrum

**Traditional Analog Advantages:**
- Generally better strong-signal handling
- Lower noise floor (premium receivers)
- No digital artifacts
- Proven reliability

### vs. SDR with Computer

**ATS-20 Advantages:**
- Standalone (no computer needed)
- Portable
- Low power consumption
- Simple operation
- Immediate on

**Computer SDR Advantages:**
- Much wider bandwidth displayed
- Recording capability
- Advanced signal processing
- Multiple demodulators simultaneously
- Superior performance (with quality hardware)

---

## Troubleshooting

### Poor Sensitivity
**Symptoms:** Can't hear weak stations
**Causes:**
- Poor antenna
- AGC set wrong
- Bandwidth too narrow
- Damaged Si4732 input

**Solutions:**
- Improve antenna (external wire)
- Check AGC settings
- Use wider bandwidth initially
- Test with strong known signal

### Frequency Error (SSB garbled)
**Symptoms:** SSB sounds like "Donald Duck"
**Causes:**
- Crystal tolerance
- Temperature drift
- BFO offset wrong

**Solutions:**
- Calibrate to known frequency
- Adjust BFO carefully
- Replace crystal with TCXO
- Allow warmup time

### Weak Audio
**Symptoms:** Low volume even at maximum
**Causes:**
- Battery voltage low
- Audio amp damaged
- Headphones high impedance
- Si4732 volume setting

**Solutions:**
- Charge battery
- Check audio amp chip
- Use 32Ω headphones
- Adjust digital volume via I2C

### I2C Communication Errors
**Symptoms:** Display frozen, no response
**Causes:**
- Poor I2C connections
- Si4732 not responding
- Arduino hung
- Power supply issue

**Solutions:**
- Check I2C pullup resistors
- Reset Arduino
- Check 3.3V supply to Si4732
- Reflash Arduino firmware

---

## Oscillator/LO Design Summary

**Key Takeaways:**

**Reference Oscillator:**
- 32.768 kHz crystal (watch crystal)
- ±20 ppm typical accuracy
- Limits SSB performance without calibration
- Low power consumption

**Frequency Synthesis:**
- Integrated PLL/VCO in Si4732
- All-digital tuning (no varicaps)
- 1 kHz steps (AM/SW), 10 kHz steps (FM)
- <50 ms lock time

**Architecture:**
- Digital Low-IF (not zero-IF)
- Single-conversion receiver
- IF: 455 kHz - 1 MHz typical
- Digital demodulation in DSP

**Performance:**
- Good for broadcast/SW listening
- Adequate for SSB with calibration
- Not competition-grade, but excellent for the price
- Frequency stability acceptable for most uses

---

## Conclusion

The ATS-20 represents an excellent example of modern receiver design using highly integrated ICs. By leveraging the Si4732-A10's integration, the design achieves:

**Strengths:**
- Extreme simplicity (few external components)
- Excellent value (~$25-35)
- Good performance for casual listening
- Portable and battery powered
- Firmware upgradeable
- SSB capable

**Limitations:**
- Not a communications-grade receiver
- Frequency accuracy marginal for SSB
- Limited strong-signal handling
- Small speaker/audio quality
- Basic display (though adequate)

**Oscillator Design Philosophy:**
- Single low-frequency crystal reference
- All synthesis on-chip
- Digital control via I2C
- Trade-off: simplicity vs. accuracy

For shortwave listeners, travelers, and emergency preparedness, the ATS-20 offers remarkable capability in a tiny package. While it won't replace a proper communications receiver for serious work, it demonstrates how modern IC integration has democratized access to multi-band radio reception.

The use of the Si4732-A10 shows the power of "radio-on-a-chip" design - what once required dozens of components and careful alignment now fits in a single IC, controlled by simple I2C commands. This represents the future of consumer and hobbyist radio receivers.

---

**References:**
- Si4732-A10 Datasheet (Silicon Labs/Skyworks)
- "AN332: Si47xx Programming Guide" - Silicon Labs Application Note
- PU2CLR Si4735 Arduino Library documentation
- ATS-20 user community forums and modifications
- Various teardowns and reverse engineering efforts
- "Digital Low-IF Receiver Architecture" - technical papers on Low-IF design