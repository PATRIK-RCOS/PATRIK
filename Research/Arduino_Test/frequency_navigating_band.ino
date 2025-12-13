// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ---------------------------- FREQUENCY ADJUSTER ----------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------




// This program is supposed to allow for navigating a band within a frequency range.
// It uses a rotary/knob connected to an Arduino Nano and breadboard.
// Pin specifications are detailed below.
// This current frequency range is set to 000.000 to 360.000 MHz, though it is adjustable.
// The amount in which you increment or decrement the frequency can also be adjusted.
// The pre-set amount is by 001.000 but can changed with a click of a button to,
//     - 000.100, 000.010, or 000.001




// ----------------------------------------------------------------------------
// ------------------------ DEFINING VARIABLES & PINS -------------------------
// ----------------------------------------------------------------------------
// ---------------- PINS ----------------
const int ENCODER_PIN_A = 2; //D2
const int ENCODER_PIN_B = 3; //D3
const int BUTTON_PIN = 5; //D5


// ---------------- VARIABLES ----------------
volatile long counter = 0; //counter
int last_pin_A_state; //state of pin A


// Variables for change detection and limits
long previous_count = 0; //last counter value


// Scaling factors
//first scale for going by 001.000   
const long SCALE_FACTOR_1   = 1L; 


//second scale for going by 000.100
const long SCALE_FACTOR_0_1 = 10L;


//third scale for going by 000.010
const long SCALE_FACTOR_0_01 = 100L;


//fourth scale for going by 000.001
const long SCALE_FACTOR_0_001 = 1000L;


// Max Step Allowed: 360 degrees * 1000 (0.001 scaling) = 360,000 steps in frequency increase
const long ABSOLUTE_MAX_COUNT = 360000L;


// Scaling Mode
// This works by 0=1.0 scale, 1=0.1 scale, 2=0.01 scale, 3=0.001 scale
int current_mode = 3;


// Debouncing the Button
long last_debounce_time = 0; //holding variable
long debounce_delay = 200; //slight delay






// ----------------------------------------------------------------------------
// -------------------------------- SET-UP ------------------------------------
// ----------------------------------------------------------------------------
void setup() {
 Serial.begin(9600);
 Serial.println("Frequency Rotary Started:"); //initial print


 pinMode(ENCODER_PIN_A, INPUT_PULLUP);
 pinMode(ENCODER_PIN_B, INPUT_PULLUP);
 pinMode(BUTTON_PIN, INPUT_PULLUP);


 last_pin_A_state = digitalRead(ENCODER_PIN_A);
 attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), updateEncoder, CHANGE);
}




// ----------------------------------------------------------------------------
// ------------------------ MODE/SCALE SWITCHING FUNC. ------------------------
// ----------------------------------------------------------------------------
void switchMode(long current_count, int old_mode) {
 // Purpose:
 //          - This function handles the switching between scaling modes
 //          - This works by clicking a button to cycle through the potential
 //            scaling.
 //              > First Click changes the scale from case 0 to case 1
 //                    = so instead of increasing by 1.000 it now will by 0.100
 //              > Second Click changes the scale from case 1 to case 2
 //                    = so instead of increasing by 0.100 it now will by 0.010
 //              > Third Click changes the scale from case 2 to case 3
 //                    = so instead of increasing by 0.010 it now will by 0.001
 //              > Fourth Click changes the scale from case 3 to case 0
 //                    = so instead of increasing by 0.001 it now will by 1.000
 //          - This will continue for as long as the button is pressed and released


 float current_freq;
 long scaling_old;
  // Getting/Figuring out which scale has been triggered.
 switch (old_mode) {
   case 0: scaling_old = SCALE_FACTOR_1;   break;
   // ^^ first scale - by 001.000
   case 1: scaling_old = SCALE_FACTOR_0_1; break;
   // ^^ second scale - by 000.100
   case 2: scaling_old = SCALE_FACTOR_0_01; break;
   // ^^ third scale - by 000.010
   case 3: scaling_old = SCALE_FACTOR_0_001; break;
   // ^^ fourth scale - by 000.001
 }
  // Calculating the Frequency
 current_freq = current_count / (float)scaling_old;
  // Cycling through the scaling modes based on clicks of the button
 // First Click: 0 -> 1
 // Second Click: 1 -> 2
 // Third Click: 2 -> 3
 // Fourth Click: 3 -> 0
 // and repeat as click more
 current_mode = (old_mode + 1) % 4;
  // The Mode's properties Setting
 long scaling_new;
 const char* mode_name;
  switch (current_mode) {
   case 0: scaling_new = SCALE_FACTOR_1;   mode_name = "1.0"; break;
   // ^^ first scale - by 001.000
   case 1: scaling_new = SCALE_FACTOR_0_1; mode_name = "0.1"; break;
   // ^^ second scale - by 000.100
   case 2: scaling_new = SCALE_FACTOR_0_01; mode_name = "0.01"; break;
   // ^^ third scale - by 000.010
   case 3: scaling_new = SCALE_FACTOR_0_001; mode_name = "0.001"; break;
   // ^^ fourth scale - by 000.001
   default: return;
 }
  // Re-Setting the global counter for the newly selected mode
 counter = (long)round(current_freq * scaling_new);
  // Preventing from going Out of Bounds or Flipping around to the other end
 if (counter < 0) counter = 0; //stop at 0 for the minimum
 if (counter > ABSOLUTE_MAX_COUNT) counter = ABSOLUTE_MAX_COUNT; //stop at max, no further


 // Re-Setting the change counter
 previous_count = counter;
  // Output status
 Serial.print("\n--- SCALE CHANGED! --- Increment: ");
 Serial.println(mode_name);
 Serial.print("Current Frequency: ");
 Serial.print(current_freq, (current_mode == 0 ? 0 : current_mode));
 Serial.println(" MHz");
 Serial.println("---------------------\n");
}






// ----------------------------------------------------------------------------
// ----------------------------------- LOOP -----------------------------------
// ----------------------------------------------------------------------------
void loop() {
 // --- Button Logic (Switching the Mode/Scaling) ---
 int button_state = digitalRead(BUTTON_PIN);
  if (button_state == LOW && (millis() - last_debounce_time) > debounce_delay) {
   switchMode(counter, current_mode);
   last_debounce_time = millis();
 }


 // --- Rotary Encoder ---
 if (counter != previous_count) {
  
   float scaled_freq;
   long scaling_current;
  
   // Determine CURRENT Mode's properties
   switch (current_mode) {
     case 0: scaling_current = SCALE_FACTOR_1;   break;
     case 1: scaling_current = SCALE_FACTOR_0_1; break;
     case 2: scaling_current = SCALE_FACTOR_0_01; break;
     case 3: scaling_current = SCALE_FACTOR_0_001; break;
     default: return;
   }
  
   // Scaling the counter to the final freq
   scaled_freq = counter / (float)scaling_current;
  
   // Attempting to prevent a greater than 360.000 value
   if (scaled_freq > 360.0) {
       scaled_freq = 360.0;
   }
  
   // Display the frequency
   Serial.print("Frequency: ");
  
   // Print with the correct number of decimal places
   if (current_mode == 0) { // 1.0
       Serial.print(scaled_freq, 0);
   } else if (current_mode == 1) { // 0.1
       Serial.print(scaled_freq, 1);
   } else if (current_mode == 2) { // 0.01
       Serial.print(scaled_freq, 2);
   } else { // 0.001
       Serial.print(scaled_freq, 3);
   }
   Serial.println(" MHz");
  
   // Update the tracker
   previous_count = counter;
 }
}






// ----------------------------------------------------------------------------
// -------------------------- UPDATING THE ENCODERS ---------------------------
// ----------------------------------------------------------------------------
void updateEncoder() {
 // Purpose: - This function stops the internal counter at 360.000
 //          - Also has Interrupts to avoid flipping between 000.000 and 360.000 when turning the rotary
 int current_pin_A_state = digitalRead(ENCODER_PIN_A);
  if (current_pin_A_state != last_pin_A_state) {
   int pin_B_state = digitalRead(ENCODER_PIN_B);
  
   if (pin_B_state != current_pin_A_state) {
     // CW - Only increment if not at the absolute maximum freq.
     if (counter < ABSOLUTE_MAX_COUNT) {
         counter++;
     }
    
   } else {
     // Only supposed to decrement if not minimum freq.
     if (counter > 0) {
       counter--;
     }
   }
   last_pin_A_state = current_pin_A_state; //setting to current
 }
}
