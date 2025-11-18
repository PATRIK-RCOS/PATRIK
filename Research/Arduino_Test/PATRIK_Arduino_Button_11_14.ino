// *****************************************************************************
// ********************* TESTING CODE FOR BUTTONS ON RADIO *********************
// *****************************************************************************

// Current Function: one button Serial Moniter will print "Pressed" when user pushes the button
// Notes on Physical Board Setup: 
//         - Button is wired from one of its pins into the pin D5 on the Arduino Nano
//         - Button is wired from another one of its pins to GND on the Arduino Nano

  
// Declaring of Variables --------------------------------------------------------------
int buttonPin = 5; //pin on Arduino (D5) for the button 
int currButton; //current state of the button (pressed or not pressed)
int lastButton = HIGH; //initial button value

// Initial Setup -----------------------------------------------------------------------
void setup() {
  Serial.begin(9600); //outputs from the connection through the USB at a 9600 baud rate
  pinMode(buttonPin, INPUT_PULLUP); //pin 5 is the input and using the internal pull up resistor
}

// Loop for Button Getting Pressed/Not Pressed -----------------------------------------
void loop() {
  currButton = digitalRead(buttonPin); //the current state of the button: LOW is pressed, HIGH is not pressed

  if (currButton == LOW && lastButton == HIGH) { //button is pressed (LOW) and the last state is not (HIGH)
    Serial.println("Pressed\n"); 
  }
  lastButton = currButton; //setting the last state to the current in preparation for the next iteration of the loop
}
