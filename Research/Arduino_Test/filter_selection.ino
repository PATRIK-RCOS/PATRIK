// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ---------------------------- FILTER SELECTION ------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


// This program is a filter selector for the radio. 
// In this design there are only 3 filters to choose from as can be seen in various 
// designs of radios on the market today.
// It is controlled by a rotary
// As the rotary rotates, the user can cycle through all possible filter options.




// ----------------------------------------------------------------------------
// ------------------- IMPORT/ DEFINING VARIABLES & PINS ----------------------
// ----------------------------------------------------------------------------
#include <RotaryEncoder.h>
const int ROTARY_CLK_PIN = 2; // D2 
const int ROTARY_DT_PIN  = 3; // D3 
const float STEPS_PER_REVOLUTION = 20.0; // for this implementation it is 20 clicks for one full turn or 360 degrees.
const float DEGREES_PER_STEP = 360.0 / STEPS_PER_REVOLUTION; //to show angle of rotary when turned
const int NO_FILTER_CODE = 0;
const int FILTER_1_CODE  = 1;
const int FILTER_2_CODE  = 2;
const int FILTER_3_CODE  = 3;
RotaryEncoder encoder(ROTARY_CLK_PIN, ROTARY_DT_PIN, RotaryEncoder::LatchMode::FOUR0); // creating the rotary object
long newPosition = 0;
long oldPosition = 0;
long totalAccumulatedSteps = 0; //steps
float currentAngle = 0.0; //initially the angle is 0 degrees though will change
int currentFilterState = NO_FILTER_CODE; // initially the filter will be at the option “No Filter”
int lastFilterState = -1; // so prints initially




// ----------------------------------------------------------------------------
// -------------------------------- SET-UP ------------------------------------
// ----------------------------------------------------------------------------
void setup() {
 Serial.begin(9600);
 Serial.println("--- Filter Selector: ---");
  // The RotaryEncoder library handles pin mode settings automatically.
}


// ----------------------------------------------------------------------------
// ----------------------------------- LOOP -----------------------------------
// ----------------------------------------------------------------------------
void loop() {
 // 1. Update encoder state
 encoder.tick();
 newPosition = encoder.getPosition();


 // 2. Check for a position change
 if (newPosition != oldPosition) {
  
   // Calculate the change and update total steps
   long delta = newPosition - oldPosition;
   totalAccumulatedSteps += delta;
  
   // 3. Calculate and Normalize the Angle
   currentAngle = totalAccumulatedSteps * DEGREES_PER_STEP;
   currentAngle = fmod(currentAngle, 360.0);
  
   if (currentAngle < 0) {
     currentAngle += 360.0;
   }


   // 4. Map the Angle to a Filter State (The Core Logic)
   // We define a 90-degree sector around each target angle (45 degrees before and 45 after).
  
   int nextFilterState = NO_FILTER_CODE; // Default is 'No Filter Selected'


   if (currentAngle < 45.0 || currentAngle >= 315.0) {
     // Sector around 0/360 degrees (315 to 45)
     nextFilterState = NO_FILTER_CODE;
   } else if (currentAngle >= 45.0 && currentAngle < 135.0) {
     // Sector around 90 degrees (45 to 135)
     nextFilterState = FILTER_1_CODE;
   } else if (currentAngle >= 135.0 && currentAngle < 225.0) {
     // Sector around 180 degrees (135 to 225)
     nextFilterState = FILTER_2_CODE;
   } else if (currentAngle >= 225.0 && currentAngle < 315.0) {
     // Sector around 270 degrees (225 to 315)
     nextFilterState = FILTER_3_CODE;
   }


   // 5. Check for State Change and Print (One-Time Logic)
   if (nextFilterState != lastFilterState) {
     currentFilterState = nextFilterState;
     printCurrentFilter();
     lastFilterState = currentFilterState;
   }
  
   // Update oldPosition for the next loop iteration
   oldPosition = newPosition;
 }
}


// ----------------------------------------------------------------------------
// ----------------------- PRINTING CURRENT FILTER ----------------------------
// ----------------------------------------------------------------------------
void printCurrentFilter() {
 //Serial.print("Angle: ");
 //Serial.print(currentAngle, 1);
 //Serial.print(" degrees | Status: ");
 switch (currentFilterState) {
   case NO_FILTER_CODE:
     Serial.println("No Filter"); //(0/360 degrees)
     break;
   case FILTER_1_CODE:
     Serial.println("Filter 1"); //(90 degrees)
     break;
   case FILTER_2_CODE:
     Serial.println("Filter 2"); //(180 degrees)
     break;
   case FILTER_3_CODE:
     Serial.println("Filter 3"); //(270 degrees)
     break;
 }
}
