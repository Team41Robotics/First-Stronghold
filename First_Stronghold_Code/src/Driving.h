/*
 * Driving.h
 *
 *  Created on: Feb 16, 2016
 *      Author: robotics_user
 */

#ifndef SRC_DRIVING_H_
#define SRC_DRIVING_H_
#include "WPILib.h"
#include <Pneumatics.h>
#define DRIVING_ENABLE
class Driving
{
private:

	CANTalon *leftTalon0;//declarations for the talons (motor controllers)
	CANTalon *rightTalon0;
	CANTalon *leftTalon1;
	CANTalon *rightTalon1;
	CANTalon *rollerTalon;

	double brightness = 0;


public:

	Talon * lightTalon;


	Driving();
	void lightOn(double);
	void getBright();
	void lightOff();
	void Move(double rightSpeed, double leftSpeed);
	void Manual_Driving(Pneumatics pneumatic, Joystick *leftJoy,Joystick *rightJoy, Joystick *controller);
};




#endif /* SRC_DRIVING_H_ */
