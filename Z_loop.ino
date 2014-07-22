// -------------------------------------------------------------------------------------------------
// the loop routine runs over and over again forever:
// -------------------------------------------------------------------------------------------------
void loop() {
  
 
  buttonCheck(); // constantly checks for joystick button press which will turn the menu on/off
  
  Serial.print(slidejoystick.smoothRead());
  Serial.print("   ");
  Serial.print(panjoystick.smoothRead());
  Serial.print("   ");
  Serial.println(tiltjoystick.smoothRead());  
  
  switch (operationState) {
    case 0: // run steppers
    
      slidestepper.step(convertAnalogToSpeed(slidejoystick.smoothRead())); // request a step with our joystick driven speed
      break;
    
    case 1: // show menu
    
      menuState = panjoystick.menuAdjust(menuState, 8, 0, 1, 200); // adjust which menu is displayed by moving joystick up/down
      
      // if we've changed menus, set our printMenu boolean to true so that we can print out our menu once
      if (menuState != prevJoy) {
        printMenu = true;
      }
      menuLCD();
      prevJoy = menuState;      
      break; 
      
    case 2: // run time lapse
    
      timeLapseEngine(); // run our time lapse engine
      break;
  }
  
}
