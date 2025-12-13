// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------- ANTENNA ROTATOR ------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


// This program simulates an antenna rotator.
// It uses a rotary and when it is turned it simulates turning an antenna tower similar to real life applications of antenna towers.


// ----------------------------------------------------------------------------
// ------------------------ DEFINING VARIABLES & PINS -------------------------
// ----------------------------------------------------------------------------
#define ENCODER_CLK_PIN 2 // D2
#define ENCODER_DT_PIN 3  // D3
#define ENCODER_SW_PIN 4  // D4
volatile int encoderSteps = 0; // the angle but in steps as rotary rotates
const int STEPS_PER_ROTATION = 20; //20 steps for 1 revolution of the rotary
volatile int lastCLKState;


// ----------------------------------------------------------------------------
// -------------------------------- SETUP -------------------------------------
// ----------------------------------------------------------------------------
void setup() {
 Serial.begin(9600);
 Serial.println("Rotary Encoder Test Initialized");
 pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
 pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
 pinMode(ENCODER_SW_PIN, INPUT_PULLUP); 
 lastCLKState = digitalRead(ENCODER_CLK_PIN);
 attachInterrupt(digitalPinToInterrupt(ENCODER_CLK_PIN), updateEncoder, CHANGE);
}


// ----------------------------------------------------------------------------
// ---------------------------- UPDATING THE ENCODER --------------------------
// ----------------------------------------------------------------------------
void updateEncoder() {
 int currentCLKState = digitalRead(ENCODER_CLK_PIN);
 if (currentCLKState != lastCLKState) { //confirming that the state is different than before (rotary moved)
   if (currentCLKState == HIGH) {
     if (digitalRead(ENCODER_DT_PIN) == LOW) { // counter clockwise rotary movement
       encoderSteps--;
       printStatus("Counter Clockwise");
     } 
     else { // clockwise
       encoderSteps++;
       printStatus("Clockwise");
     }
   }
 }
 lastCLKState = currentCLKState;
}


// ----------------------------------------------------------------------------
// ---------------------------- PRINTING --------------------------
// ----------------------------------------------------------------------------
void printStatus(const char* direction) {
 float angle = (float)encoderSteps / STEPS_PER_ROTATION * 360.0;
 int physicalSteps = encoderSteps % STEPS_PER_ROTATION; //actual rotary angle
 if (physicalSteps < 0) { //prevent negatives
   physicalSteps += STEPS_PER_ROTATION;
 }
 float physicalAngle = (float)physicalSteps / STEPS_PER_ROTATION * 360.0;
 Serial.print("Antenna is Rotating ");
 Serial.print(direction);
 Serial.print(" |");
 Serial.print(" Angle: ");
 Serial.println(physicalAngle, 1);
}


// ----------------------------------------------------------------------------
// ------------------------------------- LOOP —--------------------------------
// ----------------------------------------------------------------------------
void loop() {
 if (digitalRead(ENCODER_SW_PIN) == LOW) { // Button is pressed (LOW due to pullup)
   // Simple debouncing delay
   delay(50);
   if (digitalRead(ENCODER_SW_PIN) == LOW) {
     Serial.println("*** Button Has Been Pressed ***"); //button press for additional functions if needed for other implementations like in filter selection. (could be for locking in the angle so the antenna tower doesn’t move or some other function like that)
   }
 }
}
