// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// --------------------------- MENU NAVIGATOR (ROTARY) ------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


// This program is a menu navigator for the radio.
// It uses a rotary and a button.
// The rotary is used to go through all the options and the button is used to
// confirm a menu selection.


// ----------------------------------------------------------------------------
// ------------------------ DEFINING VARIABLES & PINS -------------------------
// ----------------------------------------------------------------------------
#define CLK_PIN 2 // D2
#define DT_PIN 3  // D3
#define SELECT_BUTTON_PIN 5 // D5
const int MIN_OPTION = 0; // options lowest value so can’t go lower than this
const int MAX_OPTION = 7; // options highest value so can’t choose above this
volatile int currentOption = 0; // currently selected option


const String MENU_OPTIONS[] = {"Home", "Push to Talk","Morse Code","Band Selection","Frequency","Filters","Volume/Squelch","Antenna Rotary"}; // all the options
const unsigned long REQUIRED_PRESS_MS = 1000; // 1000ms to use the rotary again to go through the menu options
volatile bool rotaryEnabled = true; // rotary on or off (controlled by the button) though initially on
unsigned long buttonPressStartTime = 0; // to record the button press (ms)


// ----------------------------------------------------------------------------
// ----------------------------- ROTARY READING -------------------------------
// ----------------------------------------------------------------------------
void readEncoder() {
 if (!rotaryEnabled) { //if button has the rotary turned off then can’t do anything so exit this function
   return; 
 }
 int clkState = digitalRead(CLK_PIN);
 int dtState = digitalRead(DT_PIN);
 // determining direction
 if (clkState == HIGH) { 
   if (dtState == LOW) { // dtState == LOW and clkState == HIGH means clockwise rotary movement
     currentOption++;
   } 
   else { //counter clock-wise
     currentOption--;
   }
   if (currentOption > MAX_OPTION) { // to send it back around cycling again (7->0)
     currentOption = MIN_OPTION; 
   } 
   else if (currentOption < MIN_OPTION) { //same thing but other way around (0->7)
     currentOption = MAX_OPTION; // Wrap around from 0 to 7
   }
 }
}


// ----------------------------------------------------------------------------
// -------------------- CONFIRMING MENU SELECTION (button) --------------------
// ----------------------------------------------------------------------------
void confirmSelection() {
 String selectedOption = MENU_OPTIONS[currentOption]; //stopping at wherever at so recording menu option
 rotaryEnabled = false; //now rotary is locked or turned off 
 Serial.println("-------------------------");
 Serial.print("Option Selected: ");
 Serial.println(selectedOption);
 Serial.println("Rotary Encoder Locked. Hold button for 1000ms to unlock."); //not implemented but after this print could try calling the functions created previously that correspond to the menu selection for usage.
 Serial.println("-------------------------");
}


// ----------------------------------------------------------------------------
// ------------------------------- SETUP --------------------------------------
// ----------------------------------------------------------------------------
void setup() {
 Serial.begin(9600);
 Serial.println("Navigation Menu:");
 Serial.println("Option | Index | Angle");
 pinMode(CLK_PIN, INPUT_PULLUP); 
 pinMode(DT_PIN, INPUT_PULLUP);
 pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(CLK_PIN), readEncoder, CHANGE);
 printStatus();
}


int lastPrintedOption = -1; // recording the menu option last selected


// ----------------------------------------------------------------------------
// -------------------------------- LOOP --------------------------------------
// ----------------------------------------------------------------------------
void loop() {
 if (currentOption != lastPrintedOption) { // updating the menu option
   printStatus();
   lastPrintedOption = currentOption;
 }
  // 2. Button Check and Rotary Lock/Unlock Logic
 int reading = digitalRead(SELECT_BUTTON_PIN);
 static int lastButtonState = HIGH; 
 // A. Button Press Start/Debounced Short Press (Selection)
 if (reading == LOW && lastButtonState == HIGH) {
   // Button just went from HIGH to LOW (start of a press)
   buttonPressStartTime = millis();
  
   // if the rotary is not enabled this button press (short) is not a menu selection and is going to turn on the rotary, so we skip confirmSelection().
   if (rotaryEnabled) { // selecting a menu option
     confirmSelection();
   }
 }
 if (reading == LOW && !rotaryEnabled) { // button pressed and rotary no on so possibility of turning rotary back on
   if (millis() - buttonPressStartTime >= REQUIRED_PRESS_MS) { // if 1000ms held down
     rotaryEnabled = true;
     Serial.println("Selection is Unlocked.");
   }
 }
 if (reading == HIGH && lastButtonState == LOW) { //button was let go  
      if (!rotaryEnabled && (millis() - buttonPressStartTime < REQUIRED_PRESS_MS)) {
      Serial.println("Selection is Locked."); // still locked because the rotary is still off and the button was not held or >= 1000ms
   }
   buttonPressStartTime = 0; // re-setting the recording of the button being held down
 }
 lastButtonState = reading; 
}


// ----------------------------------------------------------------------------
// ----------------------- PRINT CURRENT MENU OR STATE ------------------------
// ----------------------------------------------------------------------------
void printStatus() {
 int angle = currentOption * 45; // rotary angle
 String optionLabel = MENU_OPTIONS[currentOption]; // getting the actual string menu name
 Serial.print(optionLabel);
 Serial.print(" | ");
 Serial.print(currentOption); // Print the numeric index (0-7)
 Serial.print(" | ");
 Serial.println("");
}


