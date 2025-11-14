#include <SPI.h>

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
const unsigned long MCLK = 75000000UL; // 75 MHz crystal
const unsigned long STEP = 1000000UL;  // 1 MHz per detent
const unsigned long MAX_FREQ = 37000000UL; // Max ~MCLK/2
unsigned long frequency = 1000000UL; // Start at 1 MHz
long knobCounter = 1; // used for counting detents

// --------- Encoder State ---------
int lastCLK = HIGH;
bool buttonPressed = false;
unsigned long lastButtonPress = 0;

// --------- Functions ---------
void writeRegister(uint16_t data){
  digitalWrite(FSYNC, LOW);
  SPI.transfer16(data);
  digitalWrite(FSYNC, HIGH);
}

void setFrequency(unsigned long freq){
  if(freq > MAX_FREQ) freq = MAX_FREQ;
  if(freq < STEP) freq = STEP;

  unsigned long word = ((uint64_t)freq * (1ULL << 28)) / MCLK;

  // Set B28 = 1
  writeRegister(0x2000);

  // Load FREQ0 LSB and MSB
  writeRegister(0x4000 | (word & 0x3FFF));
  writeRegister(0x4000 | ((word >> 14) & 0x3FFF));

  // Ensure output enabled
  digitalWrite(RESET, LOW);
}

// --------- Setup ---------
void setup(){
  Serial.begin(9600);

  // Encoder pins
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);
  lastCLK = digitalRead(CLK);

  // AD9834 pins
  pinMode(FSYNC, OUTPUT);
  pinMode(SDATA, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(RESET, OUTPUT);

  // Reset pulse
  digitalWrite(RESET, HIGH);
  delay(1);
  digitalWrite(RESET, LOW);

  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE2));

  setFrequency(frequency);
}

// --------- Loop ---------
void loop(){
  int currentCLK = digitalRead(CLK);

  // Detect falling edge (one detent)
  if(lastCLK == HIGH && currentCLK == LOW){
    if(digitalRead(DT) == LOW){
      knobCounter++;
      frequency += STEP;
    } else {
      knobCounter--;
      if(knobCounter < 1) knobCounter = 1;
      frequency -= STEP;
    }

    setFrequency(frequency);
    Serial.print("Knob: ");
    Serial.print(knobCounter);
    Serial.print(" -> Frequency: ");
    Serial.println(frequency);
  }

  lastCLK = currentCLK;

  // Optional button
  if(digitalRead(SW) == LOW && millis() - lastButtonPress > 200){
    buttonPressed = true;
    Serial.println("Button pressed!");
    lastButtonPress = millis();
  } else if(digitalRead(SW) == HIGH){
    buttonPressed = false;
  }
