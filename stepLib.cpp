#include "Arduino.h"
#include "stepLib.h"

// used for declaring our motor and initializing it
stepMotor::stepMotor(byte stepPin, byte dirPin) {
  
   _stepPin = stepPin;
   _dirPin = dirPin;

   // define our digital pins as output
   pinMode(_stepPin, OUTPUT);
   pinMode(_dirPin, OUTPUT);

   // initialize our digital pins to LOW
   digitalWrite(_stepPin, LOW);
   digitalWrite(_dirPin, LOW);

   _stepCycle = false; // this keeps track of which end of the step cycle we are on: high or low
   _dir = false;
   _absPos = 0; // initialize to 0
   _lastSpeedChange = 0;
}

// function responsible for driving our digital pins high/low at the proper frequency
// input is the stepping frequency
void stepMotor::step(signed int stepFreq) {

   _time = micros(); // get the current time in microseconds      

  if (stepFreq != 0) { // we don't want to divide by 0 when joystick is at center
     _stepPeriod = 1000000 / abs(stepFreq); // get our step period (in micro-seconds) from the user given step frequency; we lose a bit of accuracy here since we've defined _stepPeriod as an unsigned long instead of a float, but that's ok...
  }
   
   // set our direction pin properly, we will only set this once for every requested direction change to save on CPU time
   if (_dir == false && stepFreq < 0) { // we were previously moving backwards, and are now requesting to move forward
     digitalWrite(_dirPin, HIGH);
     _dir = true;
   } else if (_dir == true && stepFreq > 0) { // we were previously moving forward, and are now requestion to move backward
     digitalWrite(_dirPin, LOW);
     _dir = false;
   }

   // if the proper amount of time has passed and we are requesting a speed, let's go ahead and proceed to the next half of our step cycle
   if (stepFreq != 0 && _time >= _lastStepTime + _stepPeriod) {
      digitalWrite(_stepPin, _stepCycle == true); // a compact way of writing either HIGH/LOW to our step pin based on where we are on our step cycle

      _stepCycle = !_stepCycle; // this simply flips our Boolean
     
      _lastStepTime = _time; // update the time we last stepped
      
      if (_stepCycle == true) {
        _absPos += 1 * (_dir == true) - 1 * (_dir == false); // increment our absolut position based on direction (either add or subtract)
      }
   }

}


// responsible for automatically stepping until we've reached the desired key frame position
// will apply a trapezoidal speed profile: see http://www.pmdcorp.com/news/articles/html/Mathematics_of_Motion_Control_Profiles.cfm
void stepMotor::moveToKeyframe(byte val) {
  
  // max stepping speed for this particular loop - NOTE; this will be different than the speed defined for hte main sketch because this while loop runs more quickly than the one that averages joystick signal and steps
  // NOTE - you'll want to adjust this max/min speed based on your setup
  unsigned int maxStep = 2000;
  byte minStep = 100;
  
  unsigned int accelDistance = 500; // number of steps through which to adjust speed
  unsigned long distanceTotal = abs(_absPos - _keyframeArray[val]); // total distance we need to move
  signed int hz = minStep; // initialize so that we can properly set direction
  
  
  while (_absPos != _keyframeArray[val]) { // we are not were we need to be, keep stepping; NOTE - we are using a while loop, we will stay in this loop until we reach position

    if (millis() > 10 + _lastSpeedChange) { // we only want to adjust our speed at regular intervals (10ms); this is required for trapezoidal speed profile
      if (_absPos < _keyframeArray[val]) { // adjust direction otherwise we will go the opposite way we need to
        step(-hz);
      } else {
        step(hz);
      }
    }
    
    unsigned long distanceRemaining = abs(_absPos - _keyframeArray[val]); // distance in steps we still need to move
    
    // this will apply a simple trapezoidal shape to our speed vs distance moved
    if (distanceTotal - distanceRemaining < accelDistance) { // if we need to speed up
      hz = map(distanceTotal - distanceRemaining, 0, accelDistance, minStep, maxStep); // this will automatically increase our speed based on how far we still have to move
    } else if (distanceRemaining <= accelDistance) { // we need to slow down
      hz = map(distanceRemaining, accelDistance, 0, maxStep, minStep); // this will automatically decrease our speed based on how far we still have to move
    } else {
      hz = maxStep; // cap our max speed
    }
    
    
  }
  
}


// responsible for setting the absolute position to the proper keyframe position within the keyframe array
void stepMotor::setKeyframe(byte val) {
  _keyframeArray[val] = _absPos;
}


// responsible for getting the absolute position of the requested keyframe
signed long stepMotor::getKeyframe(byte val) {
  return _keyframeArray[val];
}


// returns current absolute position of stepper
signed long stepMotor::getPos() {
  return _absPos; 
}
