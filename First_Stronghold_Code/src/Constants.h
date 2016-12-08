#pragma once

#include <math.h>
#include <random>
//Drive Controls
#define K_SWIVEL 0.8
#define K_TRANSLATE 0.8
#define K_CURVE_ADJUST 0.15
#define GRAB .9							//manual grab speed
#define LIFT 1							//manual lift speed
#define GRABBER_STALL_CURRENT 12		//stall current of grabber motor in Amps




//Connections
#define CAMERA_IP "axis-camera.local"

//#define CAMERA_ENABLE					//comment this line to remove the camera
#define COMPRESSOR_ENABLE				//comment out for competition without pcm installedß

//Joysticks

#define DRIVE_JOY 0
#define AUX_JOY 1

//Talons
#define RIGHT_TALON_A 4					//front
#define RIGHT_TALON_B 2 				//back
#define LEFT_TALON_A 1 					//back
#define LEFT_TALON_B 3 					//front
#define CENTER_TALON 5
#define LIFTER_TALON 6
#define GRABBER_TALON 7

//Analog input
#define PLASTIC_BAG 0					//hall effect sensor analog port

//DIO (Lifters)
#define LIFTER_TOP 0
#define LIFTER_ONE 2
#define LIFTER_BOTTOM 1
#define GRABBER_SWITCH 5 //TODO value

//Encoders
#define ENCODER_SOURCE_B 3
#define ENCODER_SOURCE_A 4

//Grab values
#define GRAB_ABS_TOLERANCE .03 	//TODO Recommended to be .03 for optimal performance in auton. .015 normally
#define JOY_GRAB_CLOSED 2
#define JOY_GRAB_NARROW 5
#define JOY_GRAB_CONTAINER 3
#define JOY_GRAB_WIDE 6
#define JOY_GRAB_OPEN 4

//2nd Driver Box
#define BOX_BOTTOM 4
#define BOX_ONE 5
#define BOX_TWO 6
#define BOX_THREE 3

#define BOX_TOTE 11
#define BOX_CONTAINER 12
#define BOX_GROUND 1
#define BOX_STEP 2		//if neither assume scoring platform

#define BOX_DISABLE_POT 8 		//Left Mini switch
#define BOX_DISABLE_ENCODER 7 	//Middle Mini switch
#define BOX_DISABLE_LIMITS 9 	//Right Mini switch

//Buttons
#define JOY_BUTTON_CANCEL 11
#define JOY_STACK_TOTE 12
#define JOY_TOGGLE_CURVE 8
#define JOY_GRABBER_OPEN 9
#define JOY_GRABBER_CLOSE 10

//Grabber Positions (0.0-1.0)	Values set 2015-03-8
#define GRABBER_CLOSED 0.05 //Minimum value
#define GRABBER_OPEN GRABBER_CLOSED + .830

#define GRABBER_NARROW_TOTE GRABBER_CLOSED + 0.024 //.10 all based on a .075 closed value
#define GRABBER_WIDE_TOTE GRABBER_CLOSED + 0.553 //.728
#define GRABBER_CONTAINER GRABBER_CLOSED + 0.055 //.23

#define EAT	0.1		//for canned lift function, it moves out this far from its current position

//Lift Values
#define ENCODER_TICKS_PER_REV	128
#define TEETH_PER_REV	15				//number of teeth on encoder sprocket
#define CHAIN_LENGTH	0.375			//length of chain in inches
#define VERTICAL_INCH_PER_TICK (CHAIN_LENGTH*TEETH_PER_REV)/ENCODER_TICKS_PER_REV
#define LIFT_ABS_TOLERANCE	.03			//in inches
#define LEVEL_SHIFT_INCHES 12.8			//height of tote from bottom to where new totes rest

#define INCHES_FOR_SCORING	2.0			//inches scoring platform is tall
#define INCHES_FOR_STEP	6.0				//inches scoring platform is tall
#define INCHES_FOR_CONTAINER 11.5
#define STACK_DOWN_DISTANCE	2.0			//distance the lift moves down when doing the canned pickup function

#define HEIGHT_BOTTOM 0.0//measure in inches height from floor to where we pick up 1 tote
#define HEIGHT_ONE HEIGHT_BOTTOM + LEVEL_SHIFT_INCHES
#define HEIGHT_TWO HEIGHT_BOTTOM + 2*LEVEL_SHIFT_INCHES
#define HEIGHT_THREE HEIGHT_BOTTOM + 3*LEVEL_SHIFT_INCHES
#define HEIGHT_TOP 48.5

//Grabber PID Values
#define G_PID_P -8
#define G_PID_I -0.02
#define G_PID_D 0.0

//Lifter PID Values
#define L_PID_P .8		//.8 low end Lifter
#define L_PID_I 0.07
#define L_PID_D 1.5

//Auton FUBAR Values
#define AUTON_FUBAR_FORWARD 1
#define AUTON_FUBAR_TRANSLATE .75
#define AUTON_FUBAR_TIME_FORWARD_INIT .5
#define AUTON_FUBAR_TIME_FORWARD 1.95 //1 for fubar, 1.8 for container&tote
#define AUTON_FUBAR_TIME_TRANSLATE .75
#define AUTON_FUBAR_TIME_WAIT .5
#define AUTON_FUBAR_TIME_TURN .675
#define AUTON_FUBAR_TOTE_COUNT 0;
#define AUTON_FUBAR_TIME_LASTSPIN .2
//#define AUTON_FUBAR_TIME_HOOK .5

//Auton ContainerAndToteMIDDLE() Values
#define AUTON_MID_FORWARD .75
#define AUTON_MID_TRANSLATE .75
#define AUTON_MID_TIME_FORWARD_INIT .5
#define AUTON_MID_TIME_FORWARD 2.30
#define AUTON_MID_TIME_TRANSLATE .65
#define AUTON_MID_TIME_WAIT .5
#define AUTON_MID_TIME_TURN .675
#define AUTON_MID_TIME_LASTSPIN .2
#define AUTON_MID_TOTE_COUNT 0
//#define AUTON_MID_TIME_HOOK .5

//Auton ContainerAndToteLEFT() Values
#define AUTON_LEFT_FORWARD 1
#define AUTON_LEFT_TRANSLATE .75
#define AUTON_LEFT_TIME_FORWARD_INIT .5
#define AUTON_LEFT_TIME_FORWARD 1.70 //1 for fubar, 1.8 for container&tote
#define AUTON_LEFT_TIME_TRANSLATE .65
#define AUTON_LEFT_TIME_WAIT .5
#define AUTON_LEFT_TIME_TURN .675
#define AUTON_LEFT_TIME_LASTSPIN .2
#define AUTON_LEFT_TOTE_COUNT 0
//#define AUTON_LEFT_TIME_HOOK .5

//Auton TributeToDozer
#define AUTON_TRIBUTE_FORWARD 1
#define AUTON_TRIBUTE_TIME_FORWARD 1.5

//Auton ReverseContainer
#define AUTON_RCONTAINER_BACK .75
#define AUTON_RCONTAINER_TIME_BACK 1.6

//Auton ReverseContainerTote
#define AUTON_RCONTAINERTOTE_REVERSE 1
#define AUTON_RCONTAINERTOTE_TRANSLATE 1
#define AUTON_RCONTAINERTOTE_TIME_TRANSLATE 1
#define AUTON_RCONTAINERTOTE_TIME_ACCELERATE 2
#define AUTON_RCONTAINERTOTE_TOTE_COUNT 0
#define AUTON_RCONTAINERTOTE_RATE 6 //volt/sec
