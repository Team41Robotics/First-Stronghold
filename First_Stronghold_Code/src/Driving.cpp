/*
 * Driving.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: robotics_user
 */
#include "Driving.h"
#include "WPILib.h"

Driving::Driving()
{

	leftTalon0 = new CANTalon(0);//these IDs have to be changed occasionally using the roborio-41-frc.local thing
	rightTalon0 = new CANTalon(2);
	leftTalon1 = new CANTalon(3);
	rightTalon1 = new CANTalon(1);

	lightTalon = new Talon(9);

	rollerTalon = new CANTalon(4);

}

void Driving::lightOn(double brightness)
{
	lightTalon->Set(brightness);
}

void Driving::lightOff()
{
	lightTalon->Set(0);
}

void Driving::Manual_Driving(Pneumatics pneumatic, Joystick *leftJoy, Joystick *rightJoy, Joystick *controller)
{
	bool rollerIsExtended = false;

	if(leftJoy->GetRawButton(2)||controller->GetRawButton(1))
		rollerTalon->Set(0.455);
	else if(leftJoy->GetRawButton(3)||controller->GetRawButton(3))
		rollerTalon->Set(-0.455);
	else
		rollerTalon->Set(0);


	if(rightJoy->GetRawButton(2)||controller->GetRawButton(2))
	{
		pneumatic.runRollerOut();
		rollerIsExtended = true;
	}
	else if(rightJoy->GetRawButton(3)||controller->GetRawButton(4))
	{
		pneumatic.runRollerIn();
		rollerIsExtended = false;
	}
	else
		pneumatic.turnRollerOff();
	//double throttle = ((rightJoy->GetZ() - 1) / -2);

	//sets robot speed
	double maxSpeed = 0.65;
	double minSpeed = 0.28;
	double speedRange = maxSpeed - minSpeed;
	double speedThrot = ((leftJoy->GetZ() - 1) / -2);
	double robotSpeed = ((speedThrot/1)*speedRange)+minSpeed;
//1.38461
	if(rightJoy->GetRawButton(4)&&leftJoy->GetRawButton(5))
	{
		robotSpeed *= 1.38461; // makes it max out at 0.9 in tuuuurbbooooo
	}
	SmartDashboard::PutBoolean("Roller Extended: ", rollerIsExtended);
	//printf("Throttle: %f\n", robotSpeed);

	Move((rightJoy->GetY() * robotSpeed),(leftJoy->GetY() * robotSpeed * -1));
	//printf("MANUAL DRIVING SPEED: %f",rightJoy->GetY() * robotSpeed);
	//Correct speed to roll the ball in 0.464567

}

void Driving::Move(double rightSpeed, double leftSpeed)
{
	rightTalon1->Set(rightSpeed);
	rightTalon0->Set(rightSpeed);
	leftTalon0->Set(leftSpeed);
	leftTalon1->Set(leftSpeed);
}




