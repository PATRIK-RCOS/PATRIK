// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ---------------------------- PUSH TO TALK BUTTON —--------------------------
// ----------------------------------------------------------------------------
// ---------------------------------------------------------------------------- 


// This program serves as a way to talk through the radio in the form of push to
// talk.
// This version works so when holding down the button the user is “talking” and
// upon release of the button the user is effectively “muted”.
// This design can be seen in many radios although there are other ways.






// ----------------------------------------------------------------------------
// ------------------------ DEFINING VARIABLES & PINS -------------------------
// ----------------------------------------------------------------------------
// ---------------- PINS ----------------
const int buttonPin = 5; // D5


// boolean to record if the user is “talking” or not. 
bool isTalking = false;






// ----------------------------------------------------------------------------
// -------------------------------- SET-UP ------------------------------------
// ----------------------------------------------------------------------------
void setup() {
 // Initializing the button (LOW is pressed)and the Serial prints
 pinMode(buttonPin, INPUT_PULLUP);
 Serial.begin(9600);
 Serial.println("--- Push-to-Talk System Ready ---");
}






// ----------------------------------------------------------------------------
// ----------------------------------- LOOP -----------------------------------
// ----------------------------------------------------------------------------
void loop() {
 // First reading the state of the button (pressed or not)
 int buttonState = digitalRead(buttonPin);


 // --- TALKING (button=LOW) ---
 if (buttonState == LOW) {
   // Verifying that the user is not already “talking” (button already pressed)
   if (isTalking == false) {
     // Printing "talking" to show that the user is “talking”
     Serial.println("Talking");
    
     // Setting to true now user is “talking”
     isTalking = true;
   }
 }


 // --- MUTED (button=HIGH) ---
 else { // buttonState == HIGH
   // Verifying user was just talking
   if (isTalking == true) {
     // Printing the "muted" message to show user is not “talking”
     Serial.println("Muted");
    
     // Setting to false as user is not “talking” 
     isTalking = false;
   }
 }
}
