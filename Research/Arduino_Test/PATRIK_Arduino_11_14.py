#include <SPI.h>
#include <RotaryEncoder.h>

// --------- Encoder Pins ---------
#define CLK 2
#define DT 3
#define SW 4

// --------- AD9834 Pins ---------
#define FSYNC 10
#define RESET 9
#define SDATA 11
#define SCLK 13

// --------- Parameters ---------
const unsigned long MCLK = 75000000UL;
const unsigned long STEP = 1000000UL;
const unsigned long MIN_FREQ = 1000000UL;
const unsigned long MAX_FREQ = 32000000UL;
volatile unsigned long frequency = 1000000UL;
volatile long knobCounter = 1;

unsigned long lastButtonPress = 0;

// Library instance
RotaryEncoder encoder(CLK, DT);

void writeRegister(uint16_t data){
  digitalWrite(FSYNC, LOW);
  SPI.transfer16(data);
  digitalWrite(FSYNC, HIGH);
}

void setFrequency(unsigned long freq){
  if(freq > MAX_FREQ) freq = MAX_FREQ;
  if(freq < STEP) freq = STEP;

  unsigned long word = ((uint64_t)freq * (1ULL << 28)) / MCLK;

  writeRegister(0x2000);
  writeRegister(0x4000 | (word & 0x3FFF));
  writeRegister(0x4000 | ((word >> 14) & 0x3FFF));
  digitalWrite(RESET, LOW);
}

// ISR for handling encoder movement
void checkEncoder() {
  encoder.tick(); // Call the library's tick function
}

void setup(){
  Serial.begin(9600);

  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);
  
  pinMode(FSYNC, OUTPUT);
  pinMode(SDATA, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(RESET, OUTPUT);

  digitalWrite(RESET, HIGH);
  delay(1);
  digitalWrite(RESET, LOW);

  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE2));

  setFrequency(frequency);

  // Attach interrupts to both pins
  attachInterrupt(digitalPinToInterrupt(CLK), checkEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT), checkEncoder, CHANGE);
}

int lastEncoderPosition = 0;
void loop(){
  // Check if the encoder position has changed
  int currentEncoderPosition = encoder.getPosition();
  if (abs(lastEncoderPosition - currentEncoderPosition) == 1) {
    Serial.print(lastEncoderPosition);
    Serial.print(currentEncoderPosition);
    knobCounter += (currentEncoderPosition - lastEncoderPosition);
    frequency += (currentEncoderPosition - lastEncoderPosition) * STEP;
    
    // Check bounds
    if(knobCounter < 1) knobCounter = 1;
    if(knobCounter > 32) knobCounter = 32;
    if(frequency > MAX_FREQ) 
      frequency = MAX_FREQ;
    if(frequency < STEP) 
      frequency = STEP;

    setFrequency(frequency);
    
    Serial.print("Knob: ");
    Serial.print(knobCounter);
    Serial.print(" -> Frequency: ");
    Serial.println(frequency);
    
  }
  if(digitalRead(SW) == LOW && millis() - lastButtonPress > 200){
    Serial.println("Reset frequency");
    lastButtonPress = millis();
  }
  lastEncoderPosition = currentEncoderPosition;
}
