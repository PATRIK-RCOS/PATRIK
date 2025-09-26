# Oscillators for Portable SDR HF Stations: Complete Technical Report

## Executive Summary

This report provides a comprehensive analysis of oscillator technologies suitable for portable Software Defined Radio (SDR) HF stations. The analysis covers reference oscillators, frequency synthesis methods, and practical considerations for field deployment. Key findings indicate that Temperature Compensated Crystal Oscillators (TCXO) paired with Si5351 frequency synthesizers provide the optimal balance of performance, power consumption, and cost for most portable applications.

## 1. Background

### 1.1 Software Defined Radio Overview

Software Defined Radio represents a paradigm shift in radio frequency (RF) system design, where traditional analog signal processing functions are implemented in software rather than dedicated hardware. SDR systems rely heavily on accurate, stable frequency references for several critical functions:

- **Local Oscillator (LO) Generation**: Provides the reference frequency for up/down conversion
- **Sampling Clock**: Determines the accuracy of analog-to-digital conversion
- **Digital Signal Processing**: Maintains phase coherence in complex modulation schemes
- **Frequency Accuracy**: Ensures precise tuning and calibration

### 1.2 Importance of Frequency Stability

In HF communications, frequency accuracy directly impacts:

**Reception Performance**: Frequency drift can cause signals to move outside the receiver passband, particularly problematic for narrow-band digital modes like PSK31 or FT8 where frequency accuracy requirements are typically ±10 Hz or better.

**Transmission Accuracy**: Most amateur radio frequency allocations have strict tolerances. FCC Part 97 requires frequency accuracy within ±0.01% for frequencies below 29.7 MHz, translating to ±1.44 kHz at 14.4 MHz.

**Digital Mode Operation**: Modern weak-signal protocols like MSK144 or FT4 require frequency accuracy better than ±20 Hz for reliable decoding.

### 1.3 Portable Operation Challenges

Portable SDR stations face unique constraints that differentiate them from fixed installations:

**Power Limitations**: Battery capacity restricts the use of high-power oscillator technologies like Oven Controlled Crystal Oscillators (OCXOs).

**Environmental Conditions**: Temperature variations in outdoor environments can cause significant frequency drift in uncompensated oscillators.

**Size and Weight**: Space and weight constraints favor integrated solutions over discrete component implementations.

**Setup Time**: Field operations require rapid deployment, making long warm-up times impractical.

**Cost Sensitivity**: Portable stations often serve as secondary or emergency communication systems, requiring cost-effective solutions.

### 1.4 Performance Requirements

Typical performance requirements for portable HF SDR stations include:

- **Frequency Stability**: ±1-10 ppm across operating temperature range
- **Phase Noise**: Better than -80 dBc/Hz at 10 kHz offset for most applications
- **Power Consumption**: Less than 500mA total for oscillator subsystem
- **Frequency Range**: Coverage of amateur bands from 1.8 MHz to 54 MHz minimum
- **Spurious Signals**: Better than -60 dBc for transmit applications

## 2. Types and Theory of Operation

### 2.1 Reference Oscillator Technologies

#### 2.1.1 Crystal Oscillators (XO)

**Theory of Operation**

Crystal oscillators utilize the piezoelectric properties of quartz crystals to generate precise frequencies. When an electric field is applied across a quartz crystal, it mechanically deforms. Conversely, mechanical stress generates an electric field. This property enables the crystal to act as a highly stable resonator.

The basic Pierce oscillator circuit consists of:
- Quartz crystal resonator
- Inverting amplifier (typically CMOS)
- Load capacitors (typically 18-22pF)
- Feedback resistor (1-10MΩ)

**Frequency Stability**

Uncompensated crystal oscillators exhibit frequency variation of approximately -35 ppm over the temperature range -40°C to +85°C. This variation follows a cubic curve with the turning point typically around +25°C.

**Advantages:**
- Very low power consumption (1-5mA)
- Low cost ($1-5)
- Immediate startup
- High reliability

**Disadvantages:**
- Poor temperature stability
- No frequency adjustment capability
- Limited frequency options

#### 2.1.2 Temperature Compensated Crystal Oscillators (TCXO)

**Theory of Operation**

TCXOs incorporate additional circuitry to compensate for the temperature-frequency characteristics of the crystal resonator. Modern TCXOs use one of several compensation methods:

**Analog Compensation**: Uses temperature-sensitive components (thermistors, varactors) to generate a correction voltage that counteracts the crystal's natural temperature drift.

**Digital Compensation**: Employs a temperature sensor and lookup table stored in EEPROM to generate precise correction voltages via a DAC.

**Polynomial Compensation**: Uses mathematical models to predict and correct for temperature-induced frequency variations.

**Performance Characteristics**

High-quality TCXOs achieve frequency stability of ±0.1 to ±2.5 ppm over the operating temperature range. The correction circuitry adds approximately 5-20mA to the power consumption compared to basic crystal oscillators.

**Advantages:**
- Excellent temperature stability
- Reasonable power consumption
- Fast startup time (<1 minute)
- Moderate cost ($5-25)

**Disadvantages:**
- Higher power consumption than basic XOs
- Limited frequency adjustment range
- More complex circuitry

#### 2.1.3 Voltage Controlled TCXOs (VCTCXO)

**Theory of Operation**

VCTCXOs combine the temperature compensation of TCXOs with voltage-controlled frequency adjustment. A varactor diode in series with the crystal provides frequency pulling capability, typically ±10-50 ppm from the center frequency.

The control voltage can be generated by:
- Manual adjustment potentiometer
- DAC for software control
- GPS disciplining circuits
- Automatic frequency control (AFC) loops

**Advantages:**
- Temperature compensation plus frequency adjustment
- Enables GPS disciplining
- Software frequency calibration
- Drift compensation capability

**Disadvantages:**
- Slightly higher power consumption
- Control voltage noise affects frequency stability
- More complex control requirements

#### 2.1.4 Oven Controlled Crystal Oscillators (OCXO)

**Theory of Operation**

OCXOs maintain the crystal resonator at a constant temperature (typically 70-80°C) using a proportional-integral-derivative (PID) controlled oven. By operating at a temperature above the maximum ambient temperature, the crystal experiences constant thermal conditions regardless of environmental variations.

The oven control system consists of:
- Thermally isolated crystal and oscillator circuit
- Temperature sensor (thermistor or RTD)
- Heater element (resistive)
- PID control circuit
- Insulated enclosure

**Performance Characteristics**

High-quality OCXOs achieve frequency stability of ±0.005 to ±0.1 ppm over temperature and ±0.001 ppm aging per day. However, this performance comes at the cost of high power consumption (500-2000mA) and long warm-up times (5-15 minutes).

**Advantages:**
- Exceptional frequency stability
- Low phase noise
- Excellent aging characteristics
- Wide temperature range capability

**Disadvantages:**
- Very high power consumption
- Long warm-up time
- High cost ($50-200)
- Large physical size

#### 2.1.5 GPS Disciplined Oscillators

**Theory of Operation**

GPS disciplined oscillators use the highly accurate 1 PPS (pulse per second) signal from GPS satellites as a frequency reference. The system compares the local oscillator output (divided down to 1 Hz) with the GPS 1 PPS signal and generates error corrections.

The disciplining loop typically operates as follows:
1. GPS receiver provides 1 PPS reference
2. Local oscillator output is divided to 1 Hz
3. Phase detector compares the two signals
4. Loop filter generates control voltage
5. VCTCXO frequency is adjusted to maintain lock

**Performance Characteristics**

When locked, GPS disciplined oscillators achieve frequency accuracy of ±0.001 ppm or better. During GPS signal loss, the system relies on the holdover stability of the local oscillator.

**Advantages:**
- Exceptional long-term accuracy
- Automatic calibration
- Moderate power consumption when locked
- Self-correcting for aging

**Disadvantages:**
- Requires GPS signal
- Complex control circuitry
- Acquisition time (1-10 minutes)
- Vulnerable to GPS jamming/interference

### 2.2 Frequency Synthesis Technologies

#### 2.2.1 Silicon Labs Si5351

**Architecture and Operation**

The Si5351 is a highly integrated clock generator that uses a combination of PLLs and fractional dividers to generate multiple output frequencies from a single crystal reference. The device architecture includes:

- Input reference oscillator (25-27 MHz typical)
- Two internal PLLs (PLLA and PLLB)
- Eight independent output dividers
- I2C control interface

**Frequency Generation Process:**

1. **Reference Input**: External crystal or clock input (8-100 MHz)
2. **PLL Multiplication**: Each PLL can multiply the reference by 15-90 using fractional synthesis
3. **Output Division**: Each output can divide the PLL frequency by 1-2048
4. **Final Frequency**: f_out = f_ref × (a + b/c) / R

Where:
- a = integer multiplier (15-90)
- b/c = fractional part (b: 0-1048575, c: 1-1048575)
- R = output divider (1-2048)

**Performance Specifications:**

- Output frequency range: 8 kHz to 160 MHz
- Frequency resolution: 0.01 Hz typical
- Phase noise: -80 dBc/Hz at 10 kHz offset
- Power consumption: 50-100mA
- Jitter: 1-2 ps RMS

**Advantages:**
- Multiple independent outputs
- Software programmable
- Low cost ($3-8)
- Small package size
- Good frequency resolution

**Disadvantages:**
- Moderate phase noise
- Limited to 160 MHz maximum
- Requires external reference
- Fractional spurs possible

#### 2.2.2 Silicon Labs Si570/Si571

**Architecture and Operation**

The Si570/Si571 family uses a different architecture optimized for single-output, low-jitter applications. The device contains:

- Internal CMOS oscillator
- High-speed PLL with fractional synthesis
- Low-noise output dividers
- I2C programming interface

**Key Differences:**
- **Si570**: 10-945 MHz output range
- **Si571**: 10-1400 MHz output range

**Frequency Synthesis:**

The output frequency is calculated as:
f_out = f_XTAL × (HSDIV × N1) / RFREQ

Where:
- f_XTAL = internal crystal frequency
- HSDIV = high-speed divider (4, 5, 6, 7, 9, 11)
- N1 = output divider (1-128, even values only)
- RFREQ = fractional multiplier (38-bit resolution)

**Performance Specifications:**

- Phase noise: -120 dBc/Hz at 10 kHz offset
- Jitter: <1 ps RMS typical
- Power consumption: 100-200mA
- Frequency resolution: <0.01 Hz
- Frequency stability: ±20 ppm without external reference

**Advantages:**
- Excellent phase noise performance
- Wide frequency range
- Very low jitter
- Single-output simplicity

**Disadvantages:**
- Higher power consumption than Si5351
- Single output only
- Higher cost ($15-30)
- Requires careful PCB layout

#### 2.2.3 Direct Digital Synthesis (DDS)

**Theory of Operation**

DDS chips generate sinusoidal waveforms by using digital computation and digital-to-analog conversion. The basic DDS architecture includes:

- Phase accumulator (typically 24-48 bits)
- Phase-to-amplitude converter (sine lookup table)
- Digital-to-analog converter (DAC)
- Reconstruction filter

**Frequency Generation Process:**

1. **Phase Accumulation**: A digital counter accumulates phase at each clock cycle
2. **Phase Step**: The frequency tuning word determines the phase increment per cycle
3. **Sine Conversion**: The accumulated phase addresses a sine lookup table
4. **DAC Output**: Digital amplitude values are converted to analog voltage
5. **Filtering**: Low-pass filter removes sampling artifacts

**Output Frequency Calculation:**
f_out = (FTW × f_clock) / 2^N

Where:
- FTW = frequency tuning word
- f_clock = master clock frequency
- N = phase accumulator bit width

#### AD9850 Specifications:
- Output range: DC to 40 MHz
- Resolution: 0.0291 Hz (125 MHz clock)
- Phase noise: -90 dBc/Hz at 10 kHz offset
- Power consumption: 150mA
- Spurious: -40 dBc typical

#### AD9851 Specifications:
- Output range: DC to 70 MHz (with 6× internal multiplier)
- Resolution: 0.0291 Hz
- Phase noise: -90 dBc/Hz at 10 kHz offset
- Power consumption: 150mA
- Spurious: -40 dBc typical

**Advantages:**
- Excellent frequency resolution
- Fast frequency switching
- No frequency gaps
- Phase continuity during frequency changes

**Disadvantages:**
- Spurious signal generation
- Limited frequency range
- Requires reconstruction filtering
- Clock feedthrough issues

#### 2.2.4 Phase-Locked Loop (PLL) Synthesizers

**Theory of Operation**

PLL synthesizers use feedback control to lock a voltage-controlled oscillator (VCO) to a stable reference frequency. The basic PLL consists of:

- Reference frequency divider (/R)
- Phase frequency detector (PFD)
- Charge pump and loop filter
- Voltage controlled oscillator (VCO)
- Output frequency divider (/N)

**Frequency Generation:**
f_out = f_ref × (N/R)

Modern fractional-N PLLs add a delta-sigma modulator to achieve fractional division ratios, enabling fine frequency resolution.

#### ADF4351 Specifications:
- Output range: 35 MHz to 4.4 GHz
- Resolution: Determined by reference frequency and fractional modulus
- Phase noise: -100 dBc/Hz at 10 kHz offset (typical)
- Power consumption: 100-200mA
- Spurious: -60 dBc typical

**Advantages:**
- Wide frequency range
- Low phase noise
- Integrated VCO
- SPI control interface

**Disadvantages:**
- Limited to higher frequencies (>35 MHz)
- Requires external loop filter design
- Potential for fractional spurs
- More complex programming

### 2.3 System Integration Considerations

#### 2.3.1 Clock Distribution

In multi-conversion SDR architectures, careful clock distribution is essential to maintain phase relationships and minimize spurious signals. Key considerations include:

**Clock Domain Isolation**: Different subsystems (ADC, DAC, DSP) may require different clock frequencies derived from a common reference.

**Phase Noise Degradation**: Each stage in the clock distribution chain can add phase noise. Buffer amplifiers should be chosen for low additive phase noise.

**Jitter Accumulation**: Digital processing can introduce timing jitter that degrades overall system performance.

#### 2.3.2 Power Supply Considerations

Oscillator circuits are particularly sensitive to power supply noise. Design considerations include:

**Supply Filtering**: Low-dropout (LDO) regulators with good power supply rejection ratio (PSRR)

**Decoupling**: Multiple capacitor values (0.1µF, 10µF, 100µF) to address different frequency ranges

**Ground Planes**: Solid ground planes minimize ground loops and provide low-impedance return paths

#### 2.3.3 EMI/RFI Considerations

Oscillator circuits can both generate and be susceptible to electromagnetic interference:

**Shielding**: Metal enclosures can provide 20-40 dB of isolation from external RF fields

**Output Filtering**: Low-pass filters on oscillator outputs reduce harmonic radiation

**PCB Layout**: Proper trace routing and via placement minimize coupling between circuits

## 3. Comparison Charts and Analysis

### 3.1 Reference Oscillator Comparison

| Type | Frequency Stability | Phase Noise | Power Consumption | Cost | Warm-up Time | Temperature Range |
|------|-------------------|-------------|-------------------|------|-------------|------------------|
| **Crystal (XO)** | ±10-50 ppm | Good | Very Low (1-5mA) | $1-5 | Instant | Poor |
| **TCXO** | ±0.5-2.5 ppm | Good | Low (5-20mA) | $5-25 | <1 minute | Excellent |
| **VCTCXO** | ±0.5-2.5 ppm | Good | Low (5-20mA) | $8-30 | <1 minute | Excellent |
| **OCXO** | ±0.005-0.1 ppm | Excellent | High (500-2000mA) | $50-200 | 5-15 minutes | Excellent |
| **GPS Disciplined** | ±0.001 ppm | Excellent | Medium (50-200mA) | $30-100 | 1-10 minutes | Excellent |

### 3.2 Frequency Synthesis IC Comparison

| Chip | Frequency Range | Outputs | Resolution | Phase Noise | Power | Interface | Cost | Best For |
|------|----------------|---------|------------|-------------|-------|-----------|------|----------|
| **Si5351** | 8 kHz - 160 MHz | 3 | 0.01 Hz | -80 dBc/Hz @ 10kHz | 50-100mA | I2C | $3-8 | Multi-band, Low cost |
| **Si570** | 10-945 MHz | 1 | 0.01 Hz | -120 dBc/Hz @ 10kHz | 100-200mA | I2C | $15-25 | Single LO, Good performance |
| **Si571** | 10-1400 MHz | 1 | 0.01 Hz | -120 dBc/Hz @ 10kHz | 100-200mA | I2C | $20-30 | High frequency, Single LO |
| **AD9850** | DC - 40 MHz | 1 | 0.03 Hz | -90 dBc/Hz @ 10kHz | 150mA | SPI/Parallel | $8-15 | HF DDS, Simple |
| **AD9851** | DC - 70 MHz | 1 | 0.03 Hz | -90 dBc/Hz @ 10kHz | 150mA | SPI/Parallel | $12-20 | 6m band DDS |
| **AD9912** | DC - 400 MHz | 1 | 0.001 Hz | -110 dBc/Hz @ 10kHz | 300mA | SPI | $25-40 | High resolution DDS |
| **ADF4351** | 35 MHz - 4.4 GHz | 1 | Variable | -100 dBc/Hz @ 10kHz | 100-200mA | SPI | $15-25 | VHF/UHF/Microwave |

### 3.3 Performance vs Power Analysis

| Solution | Frequency Stability | Phase Noise @ 10kHz | Total Power | Startup Time | Portable Score* |
|----------|-------------------|-------------------|-------------|-------------|----------------|
| **Si5351 + TCXO** | ±1 ppm | -80 dBc/Hz | 70mA | <1 min | 9/10 |
| **Si570 + TCXO** | ±1 ppm | -120 dBc/Hz | 120mA | <1 min | 8/10 |
| **AD9851 + TCXO** | ±1 ppm | -90 dBc/Hz | 170mA | <1 min | 7/10 |
| **Si5351 + GPS** | ±0.001 ppm | -80 dBc/Hz | 120mA | 2-5 min | 7/10 |
| **Si570 + OCXO** | ±0.01 ppm | -125 dBc/Hz | 600mA | 10 min | 5/10 |
| **AD9912 + OCXO** | ±0.01 ppm | -115 dBc/Hz | 800mA | 10 min | 4/10 |

*Portable Score considers stability, power consumption, cost, and setup time

### 3.4 Application-Specific Recommendations

#### 3.4.1 General Purpose HF SDR (Receive + QRP TX)

| Rank | Solution | Pros | Cons | Cost Range |
|------|----------|------|------|------------|
| 1 | Si5351 + 25MHz TCXO | Multiple outputs, low power, flexible | Moderate phase noise | $10-15 |
| 2 | Si570 + 10MHz TCXO | Better phase noise, proven design | Single output, higher power | $25-35 |
| 3 | AD9851 + 10MHz TCXO | Good for HF/6m, simple control | Limited frequency range | $20-30 |

#### 3.4.2 Weak Signal Work (EME, Microwave)

| Rank | Solution | Pros | Cons | Cost Range |
|------|----------|------|------|------------|
| 1 | Si570 + GPS Disciplined | Excellent stability + good phase noise | GPS dependency, complexity | $45-65 |
| 2 | ADF4351 + TCXO | Wide range, good performance | Higher frequency only | $25-40 |
| 3 | Si570 + OCXO | Best phase noise, ultimate stability | High power consumption | $70-120 |

#### 3.4.3 Multi-band Contest Station

| Rank | Solution | Pros | Cons | Cost Range |
|------|----------|------|------|------------|
| 1 | Si5351 + TCXO | 3 independent outputs, fast switching | Moderate phase noise | $10-15 |
| 2 | Dual Si570 + TCXO | Better phase noise, dual conversion | More complex, higher power | $50-70 |
| 3 | Si5351 + GPS | Long-term stability for refs | GPS dependency, startup time | $40-60 |

#### 3.4.4 Ultra-Portable (Backpack/SOTA)

| Rank | Solution | Pros | Cons | Cost Range |
|------|----------|------|------|------------|
| 1 | Si5351 + TCXO | Lowest power, small size, adequate performance | Moderate phase noise | $10-15 |
| 2 | Si570 + TCXO | Better performance, reasonable power | Single output, higher current | $25-35 |
| 3 | Internal SDR Clock | No external components | Limited performance, drift | $0 |

### 3.5 Frequency Coverage Analysis

| Solution | 160m | 80m | 40m | 20m | 15m | 10m | 6m | 2m | 70cm | Notes |
|----------|------|-----|-----|-----|-----|-----|----|----|------|-------|
| **Si5351** | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ | Max 160MHz |
| **Si570** | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | Up to 945MHz |
| **AD9851** | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ | ✗ | Max 70MHz |
| **ADF4351** | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | ✓ | ✓ | ✓ | Min 35MHz |

### 3.6 Power Budget Analysis (12V Operation)

| Component | Current Draw | Duration | Battery Impact | Notes |
|-----------|-------------|----------|----------------|-------|
| **TCXO Reference** | 10-20mA | Continuous | 240-480mAh/day | Always on |
| **Si5351** | 50-100mA | TX/Active | Variable | Can be powered down |
| **Si570** | 100-200mA | TX/Active | Variable | Can be powered down |
| **GPS Disciplined** | 50-200mA | Continuous | 1.2-4.8Ah/day | Lock maintenance |
| **OCXO** | 500-2000mA | Continuous | 12-48Ah/day | Major battery drain |

#### Recommended Battery Capacity:
- **Si5351 + TCXO**: 5-10Ah for full day operation
- **Si570 + TCXO**: 7-15Ah for full day operation  
- **GPS Disciplined**: 10-20Ah for full day operation
- **OCXO-based**: 50-100Ah for full day operation

## 4. Implementation Guidelines

### 4.1 PCB Layout Considerations

**Ground Plane Design**: Use solid ground planes with minimal splits. Oscillator circuits should have dedicated quiet ground areas isolated from switching circuits.

**Power Distribution**: Implement star power distribution with individual regulators for oscillator circuits. Use ferrite beads or small inductors to isolate digital switching noise.

**Trace Routing**: Keep oscillator traces short and direct. Use differential routing for clock signals where possible. Maintain consistent impedance (typically 50Ω single-ended, 100Ω differential).

**Component Placement**: Position oscillator circuits away from heat-generating components and high-current switching circuits. Use guard traces around sensitive analog circuits.

### 4.2 Thermal Management

**Temperature Monitoring**: Implement temperature sensors near critical oscillator components to enable software compensation or thermal shutdown.

**Heat Dissipation**: Use thermal vias under high-power components. Consider forced air cooling for high-performance applications.

**Thermal Isolation**: Isolate temperature-sensitive oscillator circuits from heat sources using thermal barriers or strategic component placement.

### 4.3 Software Control

**Frequency Programming**: Implement robust frequency calculation algorithms that account for fractional division errors and PLL lock times.

**Calibration Procedures**: Develop automated calibration routines using GPS signals or known frequency standards.

**Error Handling**: Implement comprehensive error checking for PLL lock status, temperature limits, and communication timeouts.

## 5. Conclusions and Recommendations

### 5.1 Primary Recommendations

For most portable SDR HF applications, the **Si5351 + TCXO combination** provides the optimal balance of performance, power consumption, and cost. This solution offers:

- Adequate frequency stability (±1 ppm) for amateur radio operations
- Multiple independent outputs enabling complex SDR architectures
- Low power consumption suitable for battery operation
- Fast startup time for rapid deployment
- Reasonable cost for portable/secondary stations

### 5.2 Alternative Recommendations

**Performance-Critical Applications**: Consider Si570 + TCXO for applications requiring superior phase noise performance, such as weak-signal VHF/UHF work or precision measurement.

**Ultra-Portable Applications**: For weight-critical applications like backpack portable or SOTA operations, Si5351 + TCXO remains the best choice due to minimal power requirements.

**Laboratory/Reference Applications**: GPS disciplined oscillators provide the best long-term accuracy for applications where ultimate frequency stability is required and power consumption is less critical.

### 5.3 Future Considerations

**Emerging Technologies**: Silicon photonics and MEMS oscillators may provide future alternatives with improved performance and integration.

**Software Integration**: Continued development of software-defined calibration and compensation algorithms will reduce hardware complexity requirements.

**Power Efficiency**: Ongoing improvements in low-power oscillator design will enable more sophisticated timing solutions in portable applications.

## References

1. IEEE Standards for Frequency Control, IEEE Std 1139-2008
2. Rohde, U.L., "Microwave and Wireless Synthesizers: Theory and Design," Wiley, 1997
3. Silicon Laboratories, "Si5351 Data Sheet," Rev. 1.3, 2018
4. Silicon Laboratories, "Si570/Si571 Data Sheet," Rev. 1.8, 2019
5. Analog Devices, "AD9850/AD9851 Data Sheet," Rev. E, 2004
6. FCC Part 97 - Amateur Radio Service Rules and Regulations
7. ITU-R Recommendation SM.329-12, "Unwanted emissions in the spurious domain"
8. ARRL, "The ARRL Handbook for Radio Communications," 100th Edition, 2023

