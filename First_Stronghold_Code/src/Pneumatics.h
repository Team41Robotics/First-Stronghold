/*
 * Pneumatics.h
 *
 *  Created on: Feb 13, 2016
 *      Author: robotics_user
 */

#ifndef SRC_PNEUMATICS_H_
#define SRC_PNEUMATICS_H_
#include <WPILib.h>
#include <Timer.h>

class Pneumatics {
public:
	void runCompressor();//compressor
	void runShooter();//shooting and relaxing within 1 sec of shooting combined
	void runRollerOut();//for getting the ball into the catapult dock
	void runRollerIn();
	void turnRollerOff();
	void catapultDown();
	void checkShooterStatus();//for releasing the upright catapult
	void runFan();//solenoid port 7; this will continuously be on
	Compressor *compressor;
	DoubleSolenoid *rollerSolenoid;
	DoubleSolenoid *shooterSolenoid;
	Solenoid *fan;
	Timer *timer;//timer class
	Pneumatics();
	virtual ~Pneumatics();
};

#endif /* SRC_PNEUMATICS_H_ */
