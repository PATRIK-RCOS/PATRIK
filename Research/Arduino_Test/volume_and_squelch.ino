// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// -------------------------- VOLUME/SQUELCH ROTARY ---------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


// This program works as a way to navigate up and down a band (in terms of frequency)
// This program also combines the functions of a volume and squelch into one
// rotary and one button. 
// Although it can be observed in other radio designs that the volume and squelch are 
// handled in two separate rotaries, this program only involved one.
// This is due to there only being one rotary unit available for the project at the 
// moment.






// ----------------------------------------------------------------------------
// ------------------- IMPORT/ DEFINING VARIABLES & PINS ----------------------
// ----------------------------------------------------------------------------
// ---------------- Encoder Library ----------------
#include <Encoder.h>
#define ENCODER_CLK_PIN 2 //D2
#define ENCODER_DT_PIN 3 //D3
#define BUTTON_PIN 5 //D5
Encoder myEncoder(ENCODER_CLK_PIN, ENCODER_DT_PIN);
long volumeLevel = 50;
long squelchLevel = 5;
int currentMode = 0; //volume is 0 and squelch is 1
int lastButtonState = HIGH;
long lastDebounceTime = 0;
long debounceDelay = 50; 
void printCurrentLevels(); // func. for volume/squelch level 




// ----------------------------------------------------------------------------
// -------------------------------- SET-UP ------------------------------------
// ----------------------------------------------------------------------------
void setup() {
 Serial.begin(9600); 
 pinMode(BUTTON_PIN, INPUT_PULLUP); 
 Serial.println("Volume and Squelch Rotary Control:");
 printCurrentLevels();
}






// ----------------------------------------------------------------------------
// ----------------------------------- LOOP -----------------------------------
// ----------------------------------------------------------------------------
void loop() {
 long newPosition = myEncoder.read();


 if (newPosition != 0) { //seeing if rotary is diff. than before
   if (currentMode == 0) { // VOLUME for 0
     volumeLevel += newPosition;
     volumeLevel = constrain(volumeLevel, 0, 100);
     myEncoder.write(0);    
     printCurrentLevels();


   } 
   else { // squelch for 1
     long encoderChange = newPosition;	
     if (abs(encoderChange) >= 10) { // rotary has moved
         long squelchIncrement = encoderChange / 10;
         squelchLevel += squelchIncrement;
         squelchLevel = constrain(squelchLevel, 0, 10);
         myEncoder.write(0); //re-setting
         printCurrentLevels(); 
     }
   }
 }


 // Getting button state for changing mode if applicable
 int reading = digitalRead(BUTTON_PIN); // getting the button state and info
  if (reading != lastButtonState) {
   if (reading == LOW) { // button pressed
     currentMode = 1 - currentMode; // now switch the mode (0-> 1 or 1->0)
     Serial.print("Mode changed to: ");
     Serial.println(currentMode == 0 ? "Volume" : "Squelch");
     printCurrentLevels(); 
   }
   lastButtonState = reading; 
 }
}






// ----------------------------------------------------------------------------
// -------------- PRINTING CURRENT VOLUME AND SQUELCH LEVEL FUNC. -------------
// ----------------------------------------------------------------------------
void printCurrentLevels() {
 Serial.print("Volume: "); 
 Serial.print(volumeLevel);
 Serial.print(" | Squelch: ");
 Serial.println(squelchLevel);
}
