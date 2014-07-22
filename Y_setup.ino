// -------------------------------------------------------------------------------------------------
// the setup routine runs once when you press reset:
// -------------------------------------------------------------------------------------------------
void setup() {
  LCD.begin(9600); // initialize the serial LCD
  clearScreen();
  Serial.begin(9600); // this is redundant and should be removed
  pinMode(triggerPin, OUTPUT); // initialize the digital pin as an output
  digitalWrite(triggerPin, LOW); // initialze to off position
  pinMode(joystick1Press, INPUT_PULLUP); // initialize the digital pin as an input
  digitalWrite(joystick1Press, HIGH); //setup internal pullup resistor
  pinMode(buttonPress, INPUT_PULLUP); // initialize the digital pin as an input
  digitalWrite(buttonPress, HIGH); //setup internal pullup resistor
}
