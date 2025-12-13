// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------- BAND SELECTION -------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


// This program is a band selector for possible implementation in a radio.
// It utilizes a button to toggle through a list of band options.
// The options are 80 meters, 60 meters, 40 meters, 30 meters, 20 meters, 17 meters, 
// 15 meters, 12 meters, and 10 meters.
// Corresponding to these options are the frequency ranges so when selected, the 
// users can be informed as to the range.
// The frequencies being: 3.5-4.0 MHz, 5.3305-5.3665 MHz, 7.0-7.3 MHz, 10.1-10.15 MHz, 
// 14.0-14.35 MHz, 18.068-18.168 MHz, 21.0-21.45 MHz, 24.89-24.99 MHz, 28.0-29.7 MHz.






// ----------------------------------------------------------------------------
// ------------------- IMPORT/ DEFINING VARIABLES & PINS ----------------------
// ----------------------------------------------------------------------------
const int BUTTON_PIN = 5; // Connect the push button between this pin and GND                        
const long DEBOUNCE_DELAY_MS = 50; // debounce delay 
const long LONG_PRESS_MS= 1000; //for confirming option
const char* bands[] = {"80m", "60m", "40m", "30m", "20m", "17m", "15m", "12m", "10m"}; // all the options/bands
const char* frequencies[] = {"3.5-4.0 MHz","5.3305-5.3665 MHz","7.0-7.3 MHz", "10.1-10.15 MHz","14.0-14.35 MHz","18.068-18.168 MHz","21.0-21.45 MHz","24.89-24.99 MHz","28.0-29.7 MHz"}; // all band options corresponding frequency ranges
const int NUM_BANDS = sizeof(bands) / sizeof(bands[0]);
int currentBandIndex = 0; // initial value of index in the band options
enum BandSelectionState {NO_BAND_SELECTED,BAND_SELECTING,BAND_CONFIRMED}; 
BandSelectionState currentState = NO_BAND_SELECTED;  // the current iterations band selected


// Button --------
int buttonState;
int lastButtonState = HIGH;
long lastDebounceTime = 0;
long pressStartTime = 0;


// Button Confirmation -------
bool selectionConfirmed = false;
bool confirmationPrinted = false;
bool justConfirmed = false; // so as to not reset after long-press/confirmation of band release






// ----------------------------------------------------------------------------
// -------------------------------- SET-UP ------------------------------------
// ----------------------------------------------------------------------------
void setup() {
 Serial.begin(9600);
 Serial.println("--- Band Selector: ---");
 pinMode(BUTTON_PIN, INPUT_PULLUP);
 printCurrentState();
}






// ----------------------------------------------------------------------------
// ----------------------------------- LOOP -----------------------------------
// ----------------------------------------------------------------------------
void loop() {
 int reading = digitalRead(BUTTON_PIN); // button state
 if (reading != lastButtonState) {
   lastDebounceTime = millis();
 }


 if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY_MS) {
   if (reading != buttonState) {
     buttonState = reading;


     // When pressing the button and recording the time it was pressed down
     if (buttonState == LOW) {
       pressStartTime = millis();
       justConfirmed = false;
     } 
     // When releasing the button and shorter press time (HIGH is the button)
     else { 
       if (currentState == NO_BAND_SELECTED) { // no band selected so no change except to next option
         currentState = BAND_SELECTING;
         currentBandIndex = 0;
         selectionConfirmed = false;
         confirmationPrinted = false;
         printCurrentBandPrompt();
        
       } 
     else if (currentState == BAND_SELECTING) { //selecting a button in button selecting mode
         if (!selectionConfirmed) { //if not
           currentBandIndex = (currentBandIndex + 1) % NUM_BANDS;
           printCurrentBandPrompt();
         }
        
       } 
       else if (currentState == BAND_CONFIRMED) { //confirmation of the band
         if (!justConfirmed) { // just checking that didn’t just select something so to prevent resetting the just set band
           currentState = NO_BAND_SELECTED;
           printCurrentState();
         }
       }
     }
   }
 }


 // Confirmation Button Press (Long 1000ms)
 if (buttonState == LOW && currentState == BAND_SELECTING) {
   long pressDuration = millis() - pressStartTime; // recording the time passed since holding down the button and while in the state of selecting a band


   if (pressDuration >= LONG_PRESS_MS && !selectionConfirmed) {
     // 1000ms has passed and so confirm the new band 
     currentState = BAND_CONFIRMED;
     selectionConfirmed = true;
     justConfirmed = true;


     // Now printing the confirmed output including the band and information (frequency)
     if (!confirmationPrinted) {
       Serial.print("Mode: ");
       Serial.print(bands[currentBandIndex]); 
       Serial.println("");     
       confirmationPrinted = true;
       printCurrentState();
     }
   }
 }
 if (justConfirmed && buttonState == HIGH) { // now can reset this value for next change
     justConfirmed = false;
 }


 lastButtonState = reading; // making it current 
}




// ----------------------------------------------------------------------------
// --------- PRINTING THE PROMPT OF SELECTING A BAND OR PROCEEDING ------------
// ----------------------------------------------------------------------------
void printCurrentBandPrompt() {
// this function prints the current prompt given (option of band: select or proceed)
 Serial.print("Band Selection: ");
 Serial.print(bands[currentBandIndex]);
 Serial.println("?");
 Serial.println("Hold 1000ms to confirm, or press again to get the next band option.");
}


// ----------------------------------------------------------------------------
// ----------------------- PRINTING CURRENT STATE OF USER —--------------------
// ----------------------------------------------------------------------------
void printCurrentState() {
// this function just prints out the current state depending on the currentState
// if band not selected then “No Band Selected”
// if band confirmed then it will update the current and display as such
 switch (currentState) {
   case NO_BAND_SELECTED: //none selected
     Serial.println("Band Selection: No Band Selected");
     break;


   case BAND_SELECTING: // in progress of going through options
     break;


   case BAND_CONFIRMED: // a new band has been selected
     // Print the confirmed band and its corresponding frequency
     Serial.print("Band Selection: *** ");
     Serial.print(bands[currentBandIndex]);
     Serial.print(" (");
     Serial.print(frequencies[currentBandIndex]); // <<< FREQUENCY PRINTED HERE
     Serial.print(") *** ");
     Serial.println("Press button to switch to another band.");
     break;
 }
}
