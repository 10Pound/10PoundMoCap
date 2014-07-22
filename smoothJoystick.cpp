#include "Arduino.h"
#include "smoothJoystick.h"
 
// joystick object
joystick::joystick(byte analogPin, unsigned int initialVal) {
 
  _analogPin = analogPin;
  _initialVal = initialVal;
  _bufferSize = 1024;
 
  // setup analog pins as input
  pinMode(_analogPin, INPUT);
 
  // initialize our buffer to 512 - this is the joystick neutral position
  for (int i = 0; i < _bufferSize; i++) {
    _buffer[i] = _initialVal;
  }
 
  // initialize
  _bufferIndex = 0;
  _bufferSum = _initialVal * _bufferSize;
  _lastTimeCheck = 0;
 
}
 
 
// function responsible for reading analog pin and calculating new smoothed average
unsigned int joystick::smoothRead(void) {
 
  int readAnalogVal = analogRead(_analogPin); // read our new analog value
 
  _bufferSum = _bufferSum - _buffer[_bufferIndex]; // remove our oldest analog read value from the buffer sum
  _bufferSum = _bufferSum + readAnalogVal; // add our new analog read value to buffer sum
  _buffer[_bufferIndex] = readAnalogVal; // replace old analog read value with current one
 
  // divide sum by buffer size to get our average buffer value which represents our smoothed analog value - note this division is relatively slow...
  // the notation >> X is the same as dividing by a value of X-bits, in our case 10-bit = 1024 which is the declared size of the array - this is a very fast way of dividing
  smoothedVal = _bufferSum >> 10;
 
  // increment our buffer index so we know which analog value is the oldest
  _bufferIndex++;
  if (_bufferIndex >= _bufferSize) {
    _bufferIndex = 0; // jump back to first index
  }
 
  return smoothedVal; // send our smoothed out value back to our main loop()
 
}


// helper function for adjusting menu values
// requires an initial value, min/max bounds for the value, and an increment value (value by which to increment initial value)
unsigned int joystick::menuAdjust(unsigned int input, unsigned int maxVal, unsigned int minVal, byte increment, unsigned int timeDelay) {

  unsigned int readVal = analogRead(_analogPin); // value being read from joystick

  
  if (millis() >= timeDelay + _lastTimeCheck) { // if we've waited for an interval equal to timeDelay
  
    if (readVal > 600) { // if joystick has been moved in one direction; note 400 is arbritrary, it's just some value considerably far from 512
      input += increment; // adjust input by increment
      
      // cap adjusted value at max
      if (input > maxVal) {
        input = maxVal;
      } 
    } else if (readVal < 400) { // joystick has been moved in other direction
      
      // cap adjusted value at min
      if (input == minVal) {
        input = minVal;
      } else {
        input = input - increment; // adjust input by increment
      }   
    }    
        
    _lastTimeCheck = millis(); // update last time we were here
  }
  
  return input; // send our value back to the main sketch
  
}
