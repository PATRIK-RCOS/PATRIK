// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ------------------------------ POWER ON AND OFF —---------------------------
// ----------------------------------------------------------------------------
// ---------------------------------------------------------------------------- 




// This program works as a power switch.
// When the button is initially pressed it will switch “power” on.
// When the button is pressed again it will switch “power” off and so on.
// This program is not set-up to a power source of any kind at the moment though can
// be adjusted to serve its actual intended purpose other than a print statement
// in Serial.




// ----------------------------------------------------------------------------
// ------------------------ DEFINING VARIABLES & PINS -------------------------
// ----------------------------------------------------------------------------
// ---------------- PINS ----------------
const int buttonPin = 5; // D5


// the current state of the toggle, initially set to OFF
bool isToggledOn = false;


// getting state change input which helps prevent bouncing and/or infinite printing 
int lastButtonState = HIGH; // the last reading from the input
unsigned long lastDebounceTime = 0; // the last instance of the output being toggled
long debounceDelay = 50; // debounce time






// ----------------------------------------------------------------------------
// -------------------------------- SET-UP ------------------------------------
// ----------------------------------------------------------------------------
void setup() {
 // Initializing the button pin and serial communication for printing the output
 pinMode(buttonPin, INPUT_PULLUP);
 Serial.begin(9600);
 Serial.println("--- Power On/Off Button Ready ---");
}






// ----------------------------------------------------------------------------
// ----------------------------------- LOOP -----------------------------------
// ----------------------------------------------------------------------------
void loop() {
 // First reading the current state of the button
 int reading = digitalRead(buttonPin);


 // From this: checking to see if the button has changed since the last time 
 // (on to off or off to on) or (HIGH to LOW) 
 if (reading != lastButtonState) {
   lastDebounceTime = millis(); //resetting the debouncing
 }


 // Also need to check how much time has passed since button press
 if ((millis() - lastDebounceTime) > debounceDelay) {
 
   // If the button state is LOW AND it was not the last time (HIGH):
   if (reading == LOW && lastButtonState == HIGH) {    
     // Flipping the state back to opposite
     isToggledOn = !isToggledOn; // T to F or F to T (boolean)


     // Printing the state (on or off)
     if (isToggledOn) {
       Serial.println("ON");
     } else {
       Serial.println("OFF");
     }
   }
 }


 // Preserving the current button state for the next loop iteration
 lastButtonState = reading;
}
