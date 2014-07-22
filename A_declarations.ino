/*
This script contains all of the defines and declarations for the TenPoundMoCap sketch
*/
// declare serial LCD pins
SoftwareSerial LCD(1,0 ); // RX, TX

// declare digital pin 5 as the pin that triggers our camera
#define triggerPin 12 

// declare deadband for consideration of 0 speed
#define deadband 50 

// declare our step pins
#define sliderStep 5
#define panStep 7
#define tiltStep 9

// declare our direction pins
#define sliderDir 4 
#define panDir 6 
#define tiltDir 8 

// declare speed limits for steppers measured in Hz; used for converting joystick value to a speed
#define maxSpeed 4000
#define minSpeed 1
#define TLspeed 300 // speed during stepping for time lapse

// declare joystick pins
#define joystickPin1 A1
#define joystickPin2 A2
#define joystickPin3 A3
#define joystick1Press 10 
#define buttonPress 11 

// instantiate objects
stepMotor slidestepper(sliderStep, sliderDir); // sliding stepper motor
stepMotor panstepper(panStep, panDir); // panning stepper motor
stepMotor tiltstepper(tiltStep, tiltDir); // tilting stepper motor

joystick slidejoystick(joystickPin1, 512); // instantiate slide joystick on our joystickPin, initialize center joystick value buffer: 512
joystick panjoystick(joystickPin3, 512); // instantiate panning joystick on our joystickPin, initialize center joystick value buffer: 512
joystick tiltjoystick(joystickPin2, 512); // instantiate tilting joystick on our joystickPin, initialize center joystick value buffer: 512

// variables which change through our operating code
byte cameraState = 1; // this handles where in the time lapse step order we are
byte menuState = 1; // keeps track of which menu to display
boolean printMenu = true; // allows our menu to be printed only once
byte operationState = 0; // keeps track of what 'mode' we are in - menu on, time lapse on, moving stepper
unsigned int prevJoy = 0; // keeps track of changes in joystick returned value - used to allow for printing only once to screen
unsigned int shutterOpenTime; // time at which we opened our shutter  
unsigned int moveEndTime; // time at which we've finished our interval move
unsigned int stepCounter = 0; // keep strack of how many steps we've done within our interval
unsigned int shotsTaken = 0; // current number of shots taken
unsigned int shotsRequired = 50; // number of shots to take before ending time lapse
signed int moveInterval = 2000; // number of steps to move between shots
unsigned int stabilizationDelay = 250; // time to wait, after we've moved out interval, before we open the shutter - allows our rig to settle; measured in ms
unsigned int shutterOpenDelay = 50; // time to wait before turning opto-isolator off; in bulb mode this controls our shutter speed, otherwise camera controls shutter speed
byte keyframeToSet = 1; // which keyframe number to record
byte keyframesSet = 0; // number of keyframes set by user
