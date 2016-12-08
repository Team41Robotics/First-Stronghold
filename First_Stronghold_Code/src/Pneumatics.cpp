/*
 * Pneumatics.cpp
 *
 *  Created on: Feb 13, 2016
 *      Author: robotics_user
 */
#include <Pneumatics.h>
#include <Timer.h>
Pneumatics::Pneumatics() {
	compressor = new Compressor(0-62);//compressor id (see WPILib documentation)
	shooterSolenoid = new DoubleSolenoid(0,1);//ports are 0 and 1 on PCM
	rollerSolenoid = new DoubleSolenoid(2,3);//ports are 2 and 3 on PCM
	fan = new Solenoid(7);//PCM fan
	timer = new Timer();
}
/*How the Compressor System Works
 * 		compressor loop is controlled by the SetClosedLoopControl method
 * 		When it is enabled, PCM auto turns on compressor when pressure switch is closed
 * 		Compressor off when pSwitch is open (120 psi)
 */
/*void Pneumatics::runCompressor(){
	bool enabled = compressor->Enabled();//enable the compressor
	bool pressureSwitch = compressor->GetPressureSwitchValue();//get the pressure switch value
	float current = compressor->GetCompressorCurrent();//get compressor current
	if(pressureSwitch==true){
		compressor->SetClosedLoopControl(true);
	}
	else{
		compressor->SetClosedLoopControl(false);
	}
	SmartDashboard::PutBoolean("Tanks Filled: ", pressureSwitch);
	SmartDashboard::PutNumber("Compressor Current: ", current);
}*/
void Pneumatics::runFan(){
	fan->Set(true);
}
/*Shooter State Table:
 * Resting Stage: kReverse
 * Shooting Stage: kForward
 */
void Pneumatics::runShooter(){
		double time = 1;
		shooterSolenoid->Set(DoubleSolenoid::Value::kReverse);
		//time = Timer::GetFPGATimestamp();
		timer->Reset();
		timer->Start();
		printf("FIRE!!!!!\n");
}
//2.0 was here
void Pneumatics::checkShooterStatus(){
//	if(timer->HasPeriodPassed(1))
	if(timer->HasPeriodPassed(1))
	{
			shooterSolenoid->Set(DoubleSolenoid::Value::kForward);
			timer->Stop();
	}
}
void Pneumatics::catapultDown(){
	shooterSolenoid->Set(DoubleSolenoid::Value::kForward);
}
/*Roller State Table:
 * In(Rest): kForward
 * Out: kReverse
 */
void Pneumatics::runRollerOut(){
		rollerSolenoid->Set(DoubleSolenoid::Value::kReverse);

}
void Pneumatics::runRollerIn(){
		rollerSolenoid->Set(DoubleSolenoid::Value::kForward);

}
void Pneumatics::turnRollerOff(){
		rollerSolenoid->Set(DoubleSolenoid::Value::kOff);
}
Pneumatics::~Pneumatics() {
	// TODO Auto-generated destructor stub
}

