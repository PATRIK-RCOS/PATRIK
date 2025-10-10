# PIXIE Transceiver - Technical Notes

## Overview

The PIXIE is one of the simplest transceivers possible, using approximately 8-12 transistors depending on the variant. It represents the minimalist approach to amateur radio design, making it an excellent educational platform.

**Key Specifications:**
- Frequency: Single band (crystal-determined, typically 7.023 MHz for 40m)
- Mode: CW (Morse code) only
- Power Output: 300mW - 1W typical
- Receiver: Direct conversion
- Architecture: Shared oscillator for TX/RX

---

## Circuit Architecture

### Block Diagram
```
ANTENNA
   |
   +-- T/R Switch (diode)
   |
   +--[RX]-- RF Amplifier -- Mixer -- Audio Amp -- Speaker
   |                          |
   |                      Crystal Osc
   |                          |
   +--[TX]-- Key Switch -- PA -- Low Pass Filter
```

---

## Oscillator Design (Critical Section)

### Crystal Oscillator Circuit

The heart of the PIXIE is its **Pierce crystal oscillator**, which serves dual purposes:
1. Local Oscillator (LO) for the receiver mixer
2. Carrier generator for the transmitter

**Typical Circuit:**
```
        +12V
         |
         R1 (47K)
         |
    C1 --+-- Q1 (2N3904/2N2222)
  10-30pF |     Collector
         XTAL     |
         |        R2 (470Ω)
         |        |
    C2 --+-------+-- RF Output
  10-30pF         |
         |        C3 (0.01µF) 
        GND      GND
```

**Operating Principle:**

1. **Oscillation Startup:**
   - R1 provides base bias for Q1
   - Crystal provides feedback path
   - C1 and C2 form the frequency-determining network
   - Oscillation begins at the crystal's series resonant frequency

2. **Frequency Determination:**
   - Series resonant frequency: fs = 1/(2π√(LmCm))
   - Where Lm = motional inductance, Cm = motional capacitance
   - C1 and C2 "pull" the frequency slightly (±3 kHz typical)

3. **Pulling Range:**
   - Total capacitance: CT = (C1 × C2)/(C1 + C2)
   - Frequency shift: Δf ≈ f₀ × (C₀/2CL) × (CL1 - CL2)/(CL1)
   - Where C₀ = shunt capacitance (~5pF), CL = load capacitance

**Key Design Parameters:**

- **Crystal:** 7.023 MHz fundamental mode (or other band)
- **Load Capacitance:** 20-32pF typical
- **Drive Level:** Keep under 2mW to avoid crystal damage
- **Stability:** ±100 Hz typical (temperature dependent)

**Advantages:**
- Extremely simple (3 components + transistor)
- Very stable once warmed up
- No tuning required
- Low phase noise

**Limitations:**
- Fixed frequency (can't tune across band)
- Temperature drift (±500 Hz over 0-50°C)
- Pulling range limited to ±3 kHz max
- No frequency agility

---

## Receiver Section

### Direct Conversion Mixer

**Architecture Type:** Single-balanced mixer using crystal oscillator as LO

**Circuit:**
```
RF Input --+
           |
          D1 (1N4148)
           |
        Crystal Osc (LO)
           |
          D2 (1N4148)
           |
    AF Output --[RC Filter]-- Audio Amp
```

**Operating Principle:**

The mixer multiplies the incoming RF signal with the LO:
- RF frequency: fRF ≈ 7.023 MHz
- LO frequency: fLO = 7.023 MHz (same crystal)
- Output: fRF ± fLO
- When fRF = fLO → DC (zero beat)
- When fRF = fLO ± 1kHz → 1kHz audio tone

**Why Direct Conversion Works for CW:**
- CW signals are on/off keyed carriers
- When an incoming CW signal at 7.024 MHz mixes with 7.023 MHz LO
- Result: 1 kHz audio tone (the familiar CW "beep")
- Operator can tune by slightly adjusting crystal capacitance

**Mixer Diodes:**
- Type: 1N4148 or similar small-signal switching diodes
- Function: Act as voltage-controlled resistances
- LO Power Required: ~7-10 dBm minimum

**Audio Filtering:**
- Simple RC low-pass filter (cutoff ~3 kHz)
- Removes RF components
- Passes audio frequencies for CW (300-3000 Hz)

---

## RF Front End

### RF Amplifier Stage

**Typical Configuration:**
```
Antenna -- C_coupling -- Q2 (2N3904) Common Emitter
                         |
                    Tuned Circuit (L1 + C_var)
                         |
                    To Mixer
```

**Functions:**
1. Provides RF gain (10-15 dB typical)
2. Provides some selectivity via tuned circuit
3. Improves sensitivity
4. Reduces LO radiation back to antenna

**Tuned Circuit:**
- L1: ~2-3 µH for 40m band
- Q: 20-40 typical
- Bandwidth: ~200-300 kHz
- Provides image rejection (though not critical in direct conversion)

**Limitations:**
- No strong filtering (overload from strong signals)
- No AGC (automatic gain control)
- Direct conversion creates "zero-IF" with specific issues

---

## Transmitter Section

### Crystal Oscillator as Carrier Source

Same oscillator used in receive mode becomes the carrier generator for transmit.

**CW Keying:**
```
Key Switch
    |
    +-- Oscillator Enable
    |
    +-- Power Amplifier Stage
```

**Keying Methods:**

1. **Simple On/Off Keying:**
   - Key switches oscillator power
   - Problem: Creates "clicks" (key clicks)
   - Bandwidth spreads beyond necessary

2. **Shaped Keying (Better):**
   ```
   Key -- RC Rise/Fall Circuit -- Oscillator
          (~5ms rise, ~5ms fall)
   ```
   - Softens edges
   - Reduces bandwidth
   - Less interference

### Power Amplifier

**Class A/AB Configuration:**
```
Crystal Osc -- Base Drive -- Q3 (2N2222/2N3866)
                              |
                          RFC (RF Choke)
                              |
                           +12V
                              
            Output: 500mW - 1W
```

**Design Details:**

- **PA Transistor:** 2N2222, 2N3866, or similar RF transistor
- **Bias:** Class AB for better efficiency (~40-50%)
- **Drive Power:** ~10-20 mW from oscillator
- **Output Power:** 300mW to 1W depending on supply voltage
- **Collector Load:** ~50Ω via matching network

**Impedance Matching:**
- Collector impedance: ~150-300Ω
- Antenna impedance: 50Ω
- Matching network: L-network or Pi-network

---

## Output Low-Pass Filter

### Harmonic Suppression

Critical for legal operation - must suppress harmonics below legal limits.

**5th Order Chebyshev LPF (typical):**
```
PA Output -- L1 -- C1 -- L2 -- C2 -- L3 -- Antenna
                   |            |
                  GND          GND
```

**For 40m (7 MHz):**
- L1, L3: 1.5 µH (air-wound)
- L2: 2.2 µH
- C1, C2: 330 pF (high voltage, NPO/C0G)
- Cutoff: ~10 MHz
- 2nd harmonic rejection: >40 dB
- 3rd harmonic rejection: >50 dB

**Design Goals:**
- Pass fundamental (7 MHz) with <1 dB loss
- Attenuate 2nd harmonic (14 MHz): >40 dB
- Attenuate 3rd harmonic (21 MHz): >50 dB
- Input/output impedance: 50Ω

---

## T/R Switching

### Simple Diode Switch

**Architecture:**
```
                    D_TX (1N4148)
Antenna -- C --+---/\/\/\---+-- TX PA Output
               |             |
              D_RX        RFC (blocks RF)
               |             |
           RX Input       +12V (TX key line)
```

**Operation:**

**Receive Mode:**
- TX key line = 0V
- D_TX reversed biased (open)
- D_RX forward biased (low impedance path)
- RF flows to receiver

**Transmit Mode:**
- TX key line = +12V
- D_TX forward biased (low impedance)
- D_RX reversed biased (open)
- PA output flows to antenna

**Isolation:**
- TX to RX: ~20 dB typical
- Not perfect - receiver can hear own transmit
- RX amplifier can be muted during TX (better designs)

---

## Power Supply Considerations

**Typical Requirements:**
- Voltage: 9-12V DC
- Current: 
  - Receive: 10-30 mA
  - Transmit: 100-200 mA (depending on output power)
- Regulation: Not critical, but affects frequency stability

**Oscillator Power:**
- Should be well-filtered
- Ripple affects frequency stability
- Some designs use separate regulator for oscillator (7805, etc.)

**Frequency Pulling Effects:**
- Supply voltage change → oscillator frequency change
- Typical: ~50 Hz per volt
- Battery operation preferred for stability

---

## Performance Characteristics

### Sensitivity
- Typical: 0.5-1 µV for 10 dB S/N
- Limited by:
  - Mixer conversion loss
  - LO noise
  - Audio amplifier noise
  - No RF filtering (overload)

### Selectivity
- Essentially none at RF level
- Audio filtering provides only selectivity
- Bandwidth: ~3 kHz (limited by audio filter)
- No adjacent channel rejection at RF

### Frequency Stability
- Short term: ±50-100 Hz (5 minutes)
- Long term: ±500 Hz (temperature drift)
- Key clicks: Can be severe without shaping
- Chirp: Occurs if oscillator loads during TX keying

---

## Common Issues and Solutions

### Problem: Oscillator Won't Start
**Causes:**
- Crystal defective
- Insufficient gain (wrong transistor, bad bias)
- Excessive loading on oscillator
- Wrong capacitor values

**Solutions:**
- Verify crystal with separate oscillator
- Check transistor beta (>100 typical)
- Reduce loading on output
- Adjust C1, C2 values

### Problem: Frequency Instability
**Causes:**
- Poor power supply filtering
- Thermal effects on crystal
- Mechanical vibration
- Loading changes between TX/RX

**Solutions:**
- Add voltage regulator for oscillator
- Mount crystal firmly
- Use buffer amplifier after oscillator
- Separate TX/RX oscillators (more complex)

### Problem: Severe Key Clicks
**Causes:**
- Hard on/off keying
- No shaping network
- LPF not adequate

**Solutions:**
- Add RC shaping (5ms rise/fall)
- Verify LPF construction
- Consider buffered keying

### Problem: Low TX Power
**Causes:**
- Insufficient drive from oscillator
- Poor impedance matching
- Low supply voltage
- PA transistor damaged

**Solutions:**
- Verify oscillator output level (~10-20 mW)
- Check/redesign matching network
- Verify 12V supply under load
- Replace PA transistor

---

## Improvements and Variants

### VFO Version (PIXIE-VXO)
Replace crystal oscillator with Variable Crystal Oscillator (VXO):
- Add variable capacitor in series with crystal
- Tuning range: ±10 kHz typical
- Allows working multiple stations
- Still very simple (add one variable capacitor)

### Superheterodyne PIXIE
Some builders add IF stage:
- Crystal filter for selectivity
- Better sensitivity
- More complex (defeats "simplicity" goal)

### Two-Crystal Design
Use separate crystals for TX and RX:
- Eliminates chirp
- Allows offset for better zero-beating
- Doubles crystal cost

---

## Conclusion

The PIXIE represents the absolute minimum viable transceiver design. Its single crystal oscillator serves both transmit and receive functions, demonstrating elegant simplicity. While it has significant limitations (fixed frequency, no selectivity, low power), it serves as an excellent introduction to:
- Crystal oscillator design and behavior
- Direct conversion receiver principles
- Basic RF power amplifier design
- CW operation
- RF construction techniques

The oscillator design, while extremely simple, demonstrates core principles that apply to much more sophisticated radios: frequency stability, phase noise, and the trade-off between simplicity and performance.

---

**References:**
- ARRL Handbook (various editions, 2014, 2017, 2020)
- "Experimental Methods in RF Design" - ARRL
- QRP ARCI publications