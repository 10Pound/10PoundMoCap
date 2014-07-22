#ifndef _stepLib_h_
#define _stepLib_h_

#include "Arduino.h";

// define our stepper class
class stepMotor {
   public:
      stepMotor(byte stepPin, byte dirPin); // our stepper object with variables stepPin and dirPin
      void step(signed int stepFreq); // our stepping function which takes as an input our stepping frequency
      void setKeyframe(byte val); // allows us to store keyframes in array
      signed long getKeyframe(byte val); // returns absolute position of keyframe
      signed long getPos(); // returns current position of stepper
      void moveToKeyframe(byte val); // automatically moves stepper to given keyframe
   private:
     unsigned long _time; // current time
     unsigned long _lastStepTime; // time at which we last stepped
     unsigned long _stepPeriod; // time between a half period - this is the same as our delay(X) of part 1
     unsigned long _lastSpeedChange; // time at which we adjusted our stepping frequency
     byte _stepPin;
     byte _dirPin;
     boolean _stepCycle; // defines if we are on the HIGH or LOW side of our step cycle
     boolean _dir; // keeps track of which direction we're moving
     signed long _keyframeArray[10]; // only store up to 10 keyframes
     signed long _absPos; // current absolute position of our stepper; NOTE; when you turn power on, this is position = 0
};


#endif
