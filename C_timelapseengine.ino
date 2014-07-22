// -------------------------------------------------------------------------------------------------
// non-blocking function responsible for triggering our camera and our motor move at the proper time
// -------------------------------------------------------------------------------------------------
void timeLapseEngine() {
  
  switch (cameraState) {
    case 0: // we've finished taking our shots, do nothing
      tlComplete(); //LCD prints "TL Complete!"
      slidestepper.moveToKeyframe(1); // move to our first keyframe
      operationState = 0; // take us out of time lapse mode and back into stepper moving mode
      break;
    
    case 1: // opening shutter case
    
      digitalWrite(triggerPin, HIGH); // open shutter
      shutterOpenTime = millis(); // get time
      cameraState++; // proceed to next step
      shutopenAt(); //LCD prints "Shutter open at" on line 1 and value of shutterOpenTime variable on line 2
      break; 
      
    case 2: // closing shutter case
    
      if (millis() >= shutterOpenTime + shutterOpenDelay) { // check to see if shutter has been left open long enough
        digitalWrite(triggerPin, LOW); // close shutter
        cameraState++; // proceed to next step
        shotsTaken++; // increment shots taken
        //Serial.print("Shutter closed at "); Serial.print(millis()); Serial.print(", shots taken: "); Serial.println(shotsTaken);
        
        if (shotsTaken == shotsRequired) { // if we've taken all the shots we need
          cameraState = 0; // stop SMS TL mode
        }
      }    
      break;
      
    case 3: // step through our interval
    
      // step our motor with proper direction
      if (slidestepper.getKeyframe(2) < slidestepper.getKeyframe(1)) {
        slidestepper.step(TLspeed);
      } else {
        slidestepper.step(-TLspeed);
      }

      stepCounter++;
      //Serial.print("Position "); Serial.println(stepCounter);
      
      if (stepCounter == moveInterval) { // we've moved the number of steps required
        cameraState++; // proceed to next step
        stepCounter = 0; // reset our counter
        moveEndTime = millis(); // get time
        //Serial.print("Finished moving our distance interval at time "); Serial.println(moveEndTime);
      }    
      break;
      
    case 4: // wait for rig to stabilize
    
      if (millis() >= moveEndTime + stabilizationDelay) { // check to see if we've waited for stabilizationDelay time
        cameraState = 1; // proceed back to first step
        //Serial.print("Finished stabilizing at time "); Serial.println(millis());
      }    
      break;      
      
  }
 
}
