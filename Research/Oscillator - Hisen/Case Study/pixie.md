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

**References:**
- ARRL Handbook (various editions, 2014, 2017, 2020)
- "Experimental Methods in RF Design" - ARRL
- QRP ARCI publications