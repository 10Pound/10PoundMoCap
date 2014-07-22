// -------------------------------------------------------------------------------------------------
// responsible for:
// - printing proper menu label
// - allowing for the adjustment of menu values
// - starting time lapse
// -------------------------------------------------------------------------------------------------
void menuLCD() {
  
  switch (menuState) {
    case 0: // handles wrapping around
      menuState = 7;
      break;
    
    case 1: // set keyframe
      if (printMenu) {
        lcdsetKeyframe(); //LCD prints "Set Keyframe:"
        printMenu = false;
      }        
      
      if (buttonCheck() == 1) { // check for button press so we know when user has chosen "OK"
        slidestepper.setKeyframe(keyframeToSet);
        keyframeToSet++; // increment which keyframe we want to set
        keyframesSet++; // increment the number of keyframes set
        lcdkeyframeSet(); //LCD prints "keyframe set!"
        operationState = 0; // turn off menu so that we can continue moving steppers
        printMenu = true;
        delay(1000);  // allow user to read response
      }
      
      prevJoy = slidejoystick.menuAdjust(keyframeToSet, 2, 1, 1, 100); // adjust menu value by moving joystick left/right
      if (prevJoy != keyframeToSet) { // check previous against new value, if different - set boolean so we can print to menu
        keyframeToSet = prevJoy;
        printMenu = true;
      }
      break; 
      
    case 2: // number of shots to take in TL
        if (printMenu) {
        lcdnumberShots();
        printMenu = false;
      }
      
      prevJoy = slidejoystick.menuAdjust(shotsRequired, 1000, 1, 1, 100); // adjust menu value by moving joystick left/right
      if (prevJoy != shotsRequired) { // check previous against new value, if different - set boolean so we can print to menu
        shotsRequired = prevJoy;
        printMenu = true;
      }
      break;
      
    case 3: // move increment; calculated automatically if two keyframes are set
        if (printMenu) {
        lcdmoveIncrements(); //LCD prints "Move incr."
        if (keyframesSet == 2) { // if we have 2 keyframes set, we can automatically calculate the move interval
          lcdautocalcmoveIncr(); //LCD prints "auto - "
          moveInterval = abs(slidestepper.getKeyframe(2) - slidestepper.getKeyframe(1)) / (shotsRequired - 1); // note this returns an int, so we lose a bit of accuracy, but we can't do less than one step anyway
          lcdmoveInterval(); //LCD prints value of moveInterval variable
        } else { // otherwise we can allow for a manual one - NOTE: you may move further than your slider length
          lcdmoveInterval(); //LCD prints value of moveInterval variable
        }
        printMenu = false;
      }
      
      if (keyframesSet != 2) { // we only need to adjust increment when it's not auto
        prevJoy = slidejoystick.menuAdjust(moveInterval, 10000, 1, 10, 100); // adjust menu value by moving joystick left/right
      } else {
        prevJoy = moveInterval;
      }
      
      if (prevJoy != moveInterval) { // check previous against new value, if different - set boolean so we can print to menu
        moveInterval = prevJoy;
        printMenu = true;
      }
      break;
      
    case 4: // shutter open time
        if (printMenu) {
        lcdshutterOpen();
        printMenu = false;
      }
        
      prevJoy = slidejoystick.menuAdjust(shutterOpenDelay, 5000, 0, 10, 100); // adjust menu value by moving joystick left/right
      if (prevJoy != shutterOpenDelay) { // check previous against new value, if different - set boolean so we can print to menu
        shutterOpenDelay = prevJoy;
        printMenu = true;
      }
      break;
      
    case 5: // stabilization time
      if (printMenu) {
        lcdStabilization();
        printMenu = false;
      }

      prevJoy = slidejoystick.menuAdjust(stabilizationDelay, 5000, 0, 10, 100); // adjust menu value by moving joystick left/right
      if (prevJoy != stabilizationDelay) { // check previous against new value, if different - set boolean so we can print to menu
        stabilizationDelay = prevJoy;
        printMenu = true;
      }
      break;
      
    case 6: // current position
        if (printMenu) {
        lcdcurrentPos();
        printMenu = false;
      }
      break;
      
    case 7: // start time lapse
        if (printMenu) {
        lcdstartTL();   
        printMenu = false;
      }      
      
      if (buttonCheck() == 1) { // check for button press so we know when user has chosen "OK"
        lcdstartTLOK(); //LCD prints "Start TL!" - report back to user that we've set keyframe
        cameraState = 1; // reset in case we previously ran TL
        shotsTaken = 0; // reset in case we previously ran TL
        operationState = 2; // turn menu off and activate TL
        menuState = 1; // go back to first menu in case we pull it back up
        slidestepper.moveToKeyframe(1); // move to our first keyframe
        delay(1000);  // allow user to read response
      }
      break;
   
    case 8: // handles wrapping around
      menuState = 1;
      break;    
  }
}
//-------------------------------------------------------------------------------------------
void clearScreen()
{
  //clears the screen
  LCD.write(0xFE);
  LCD.write(0x01); 
}
//-------------------------------------------------------------------------------------------
void selectLineOne()
{ 
  //puts the cursor at line 0 char 0.
  LCD.write(0xFE); //command flag
  LCD.write(128); //position
}
//-------------------------------------------------------------------------------------------
void selectLineTwo()
{ 
  //puts the cursor at line 0 char 0.
  LCD.write(0xFE); //command flag
  LCD.write(192); //position
}
//-------------------------------------------------------------------------------------------
void turnDisplayOff()
{
  //this tunrs the display off, but leaves the backlight on. 
  LCD.write(0xFE); //command flag
  LCD.write(8); // 0x08
}
//-------------------------------------------------------------------------------------------
void turnDisplayOn()
{
  //this turns the dispaly back ON
  LCD.write(0xFE); //command flag
  LCD.write(12); // 0x0C
}
//-------------------------------------------------------------------------------------------
void boxCursorOn()
{
  //this turns the box cursor on
  LCD.write(0xFE); //command flag
  LCD.write(13); // 0x0D
}
//-------------------------------------------------------------------------------------------
void boxCursorOff()
{
  //this turns the box cursor off
  LCD.write(0xFE); //command flag
  LCD.write(12); // 0x0C
}
//-------------------------------------------------------------------------------------------
void lcdsetKeyframe()
{
  clearScreen();
  selectLineOne();
  LCD.print("Set keyframe: ");
  selectLineTwo();
  LCD.print(keyframeToSet);
  delay(500); 
}
//---------------------------------------------
void lcdkeyframeSet()
{
  clearScreen();
  selectLineOne();
  LCD.print("Keyframe set!");
  delay(500); 
}
//--------------------------------------------
void lcdnumberShots()
{
  clearScreen();
  selectLineOne();
  LCD.print("Num Shot:");
  selectLineTwo();
  LCD.print(shotsRequired);
  delay(500); 
}
//--------------------------------------------
void lcdmoveIncrements()
{
  clearScreen();
  selectLineOne();
  LCD.print("Move incr.");
  delay(500); 
}
//-------------------------------------------
void lcdautocalcmoveIncr()
{
  clearScreen();
  selectLineOne();
  LCD.print("auto - ");
  delay(500); 
}
//-------------------------------------------
void lcdmoveInterval()
{
  clearScreen();
  selectLineOne();
  LCD.print(moveInterval);
  delay(500); 
}
//-------------------------------------------
void lcdshutterOpen()
{
  clearScreen();
  selectLineOne();
  LCD.print("Shut. open");
  selectLineTwo();
  LCD.print(shutterOpenDelay);
  delay(500); 
}
//-------------------------------------------
void lcdStabilization()
{
  clearScreen();
  selectLineOne();
  LCD.print("Stabil.");
  selectLineTwo();
  LCD.print(shotsRequired);
  delay(500); 
}
//------------------------------------------
void lcdcurrentPos()
{
  clearScreen();
  selectLineOne();
  LCD.print("Curr. Pos.");
  selectLineTwo();
  LCD.print(slidestepper.getPos());
  delay(500); 
}
//-----------------------------------------
void lcdstartTL()
{
  clearScreen();
  selectLineOne();
  LCD.print("Start TL");
  delay(500); 
}
//-----------------------------------------
void lcdstartTLOK()
{
  clearScreen();
  selectLineOne();
  LCD.print("Start TL!");
  delay(500); 
}
//-----------------------------------------
void tlComplete()
{
  clearScreen();
  selectLineOne();
  LCD.print("TL Complete!");
  delay(500); 
}
//------------------------------------------
void shutopenAt()
{
  clearScreen();
  selectLineOne();
  LCD.print("Shutter open at");
  selectLineTwo();
  LCD.print(shutterOpenTime);
  delay(500); 
}
//------------------------------------------
