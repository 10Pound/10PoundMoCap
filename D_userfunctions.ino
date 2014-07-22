// -------------------------------------------------------------------------------------------------
// responsible for monitoring button presses
// joystick button returns 2
// push button returns 1
// otherwise returns 0
// -------------------------------------------------------------------------------------------------
byte buttonCheck() {

  if (digitalRead(joystick1Press) == 0) { // joystck button is pressed
    
    operationState = !operationState; // toggle menu on/off
    
    if (!operationState) { // if menu was turned off, reset the switch case for it so that we always start at the same menu position
      menuState = 1;
    }
    
    printMenu = true; // so that we can print the menu once again
    
    delay(300); // it's ok to delay here since i'm assuming you aren't moving your stepper and pressing this button at the same time - we need this to slow down the boolean being switched on/off
    return 2; 
  } else if (digitalRead(buttonPress) == 0) { // push button is pressed
    delay(300);
    return 1;
  } else {
    return 0;
  }
}





// -------------------------------------------------------------------------------------------------
// this function takes as an input, an unsigned analog value and will convert it into a signed value which represents stepper speed
// the output is bounded by a min and max stepper speed
// -------------------------------------------------------------------------------------------------
signed int convertAnalogToSpeed(unsigned int analogVal) {
 
  if (analogVal < 512 + deadband && analogVal > 512 - deadband) { // we are within deadband, return a 0 speed
    return 0;
  } else if (analogVal >= 512 + deadband) { // move in forward direction - return a positive speed
    return map(analogVal, 512 + deadband, 1024, minSpeed, maxSpeed); // interpolate our joystick value to the proper speed
  } else if (analogVal <= 512 - deadband) { // move in reverse direction - return a negative speed
    return -map(analogVal, 512 - deadband, 0, minSpeed, maxSpeed); // interpolate our joystick value to the proper speed
  }
 
}

