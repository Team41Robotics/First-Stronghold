#include "WPILib.h"
#include <SpeedController.h>
#include <Talon.h>
#include <Driving.h>
#include <Vision41.h>
#include "Constants.h"
#include <fstream>
#include <Pneumatics.h>

//this is for both manual and auto

#define ROBOTSPEED 0.5
#define AREA_MINIMUM 150
#define JOYRIGHT 1
#define JOYLEFT 0

class Robot: public IterativeRobot {
#ifdef VISION_AUTO_ENABLE//(defined(VISION_AUTO_ENABLE) || defined(VISION_ENABLE))
	Vision41 *VisionTargeting;
#endif
private:
	SendableChooser *autonModeChoose; //this is needed, we don't know why
	Command *autonCommand;

	CameraServer *cameraServer;

	Joystick *leftJoy; //declarations for the joysticks
	Joystick *rightJoy;

	Joystick * controller;

	Pneumatics pneumatic;
#ifdef DRIVING_ENABLE
	Driving *driving;
#endif
	bool autonDrivingFinished = false;bool autonIsFinished = false;bool autonShootingFinished =
			false;bool autonReadyToFire = false;
	double position = 1;
	double brightness = 0;bool onTarget = false;bool joystickTriggerBool = false;
	float joyXVal;
	float joyYVal;
	float joyZVal;

	double lTrig = 0;
	double rTrig = 0;
	Timer *timer;
	LiveWindow *lw;
	//int guo = 0;//our not failsafe failsafe.

	void RobotInit() {
		//lw = LiveWindow::GetInstance();
		timer = new Timer();
		//create the joystick
		leftJoy = new Joystick(JOYLEFT); //this is called using a constructor
		rightJoy = new Joystick(JOYRIGHT);
		controller = new Joystick(2);
#ifdef DRIVING_ENABLE
		driving = new Driving();
#endif
#ifdef VISION_AUTO_ENABLE
		VisionTargeting = new Vision41();
#endif

		/* 	TODO: add gyro initialization
		 *	accel = new BuiltInAccelerometer(Accelerometer::kRange_8G);
		 zGyro = new AnalogGyro(0);
		 zGyro->SetSensitivity(0.007); //7 mV per degree per second
		 zGyro->Calibrate ();
		 */

		//driving->lightOn(0.5);
		//cameraServer = CameraServer::GetInstance();
		//cameraServer->SetQuality(50);
		//cameraServer->StartAutomaticCapture("cam1");
	}

	void AutonomousInit() {
		timer->Reset();
		timer->Start();
#ifdef DRIVING_ENABLE
		driving->Move(0, 0);
#endif
#ifdef VISION_AUTO_ENABLE
		VisionTargeting->AutoCam();
		VisionTargeting->pidReset();
#endif

	}

	void AutonomousPeriodic() {
		//static double targetFirstSeen = 9999;
		//static bool onTargetLatch = false;
		pneumatic.checkShooterStatus();
		if (autonDrivingFinished == false) {
			onTarget = false;
			pneumatic.runFan();

			pneumatic.runRollerIn();

			Wait(1);

			pneumatic.runRollerOut();

			//do low bar things
			//TODO:
//2.0 was here
#ifdef DRIVING_ENABLE
			//driving->Move(-1, 1);//FULL STEAM AHEAD (was 0.65 for violent siege mode DESTROY HOWEVER WE WENT TOO FAR FOWARD. 0.55 is now standard.)
			driving->Move(-0.55, 0.55);
#endif
			//Wait(1.55);
			Wait(3);
#ifdef DRIVING_ENABLE
			driving->Move(0, 0);
#endif
			Wait(0.5);
#ifdef DRIVING_ENABLE
			driving->Move(0.56, 0.56);			//turn
#endif
			Wait(0.4);			//halve it for rox
#ifdef DRIVING_ENABLE
			driving->Move(0, 0);
#endif
			autonDrivingFinished = true;
			//VisionTargeting->error = 0.02;//may be issuesss
		}

		if (!autonShootingFinished) {

#ifdef VISION_AUTO_ENABLE
			driving->lightOn(0.3);
			VisionTargeting->AutoAim();
			driving->Move(-VisionTargeting->out, -VisionTargeting->out);
			//if(guo>0.01)
			//guo=0.01;
			if (fabs(VisionTargeting->error) < 0.026 && timer->Get() > 10
					&& VisionTargeting->numTargets != 0) //&& fabs(VisionTargeting->error) > 0.0/*guo*/)//test with 0 error and if that doesn't work lets force it to wait?
							{
				//if (onTargetLatch == false)
				//{
				//targetFirstSeen = timer->Get();
				//onTargetLatch = true;
				//}
				//if (onTargetLatch && timer->Get() - targetFirstSeen > .5)
				//{
				autonReadyToFire = true;
				autonShootingFinished = true;
				//}
			}

			//else
			//onTargetLatch = false;
			//guo=guo+0.00005; //maybe make this smaller. Estimated loop time is once every 30 ms so -shrugs-
#endif
		}
		if (autonReadyToFire == true) {
#ifdef VISION_AUTO_ENABLE
			driving->Move(0, 0);
			pneumatic.runRollerIn();
			Wait(1);
			printf("FIRE FIRE FIRE FIRE KAPPA");
			SmartDashboard::PutNumber("CMXN",
					VisionTargeting->centerMassXNormalized);
			SmartDashboard::PutNumber("Error", VisionTargeting->error);

			pneumatic.runShooter();
			VisionTargeting->pidReset();
			driving->Move(0, 0);
			autonReadyToFire = false;
#endif
		}
	}

	void TeleopInit() {
#ifdef DRIVING_ENABLE
		driving->lightOff();
#endif
		autonDrivingFinished = false;
		autonIsFinished = false;
		autonShootingFinished = false;
		autonReadyToFire = false;
#ifdef DRIVING_ENABLE
		driving->Move(0, 0);
#endif

//#ifdef VISION_AUTO_ENABLE
		VisionTargeting->AutoCam();
//#endif
		//TODO: remember we need this on the field
	}

	void TeleopPeriodic() {

		pneumatic.runFan();
		pneumatic.checkShooterStatus();
		//add psi readout
		//driving->lightOn(.3);
		//VisionTargeting->AutoAim();
		//SmartDashboard::PutNumber("CMXN",VisionTargeting->centerMassXNormalized);
		//SmartDashboard::PutNumber("Error",VisionTargeting->error);
		//if(fabs(VisionTargeting->error) < 0.01)
		//onTarget = true;
		//else
		//onTarget = false;
		//SmartDashboard::PutBoolean("On Target:",onTarget);

		//second driver://A1 B2 X3 Y4

		if (rightJoy->GetRawButton(1) || leftJoy->GetRawButton(1)) {
			//auto
			//driving->Move(0,0);
			driving->lightOn(.3);
#ifdef VISION_ENABLE
			VisionTargeting->AutoAim();

			SmartDashboard::PutNumber("CMXN",
					VisionTargeting->centerMassXNormalized);
			SmartDashboard::PutNumber("Error", VisionTargeting->error);
			if (fabs(VisionTargeting->error) < 0.01)
				onTarget = true;
			else
				onTarget = false;
			VisionTargeting->aspectTarget();

			SmartDashboard::PutBoolean("On Target:", onTarget);

			//if(VisionTargeting->camera->IsFreshImage()&&rightJoy->GetRawButton(5))
			if (rightJoy->GetRawButton(5)) {
				driving->Move(-VisionTargeting->out, -VisionTargeting->out);
				//printf("VISION TARGETING SPEED: %f",VisionTargeting->out);
			} else {
				VisionTargeting->pidReset();
			}
#endif

			if (rightJoy->GetRawButton(1) && leftJoy->GetRawButton(1)) {
				//fires the ball

				//TODO:check if extended
				pneumatic.runRollerIn();
				Wait(0.75);
				pneumatic.runShooter();
#ifdef VISION_ENABLE
				VisionTargeting->pidReset();
#endif
			}
		} else {
			driving->lightOff();
			//Manual Driving
#ifdef VISION_ENABLE
		//	VisionTargeting->pidReset();
#endif

#ifdef DRIVING_ENABLE
			driving->Manual_Driving(pneumatic, leftJoy, rightJoy, controller);
#endif
			if (leftJoy->GetRawButton(7))
				pneumatic.catapultDown();
#ifdef VISION_AUTO_ENABLE //Needs to be always enabled if auto targetting was ever used. Hard reset
			if (leftJoy->GetRawButton(4))
				VisionTargeting->pidReset();
#endif


			lTrig = controller->GetRawAxis(2);
			rTrig = controller->GetRawAxis(3);

			if (lTrig > 0.75 || rTrig > 0.75) {
				driving->lightOn(.3);
#ifdef VISION_ENABLE
				VisionTargeting->AutoAim();

				SmartDashboard::PutNumber("CMXN",
						VisionTargeting->centerMassXNormalized);
				SmartDashboard::PutNumber("Error", VisionTargeting->error);
				if (fabs(VisionTargeting->error) < 0.01)
					onTarget = true;
				else
					onTarget = false;
				VisionTargeting->aspectTarget();

				SmartDashboard::PutBoolean("On Target:", onTarget);
				if (controller->GetRawButton(5)) {
					driving->Move(-VisionTargeting->out, -VisionTargeting->out);
					//printf("VISION TARGETING SPEED: %f",VisionTargeting->out);
				} else {
					VisionTargeting->pidReset();
				}

#endif

				if (lTrig > 0.75 && rTrig > 0.75) {
					//fires the ball

					//TODO:check if extended
					pneumatic.runRollerIn();
					Wait(0.75);
					pneumatic.runShooter();
#ifdef VISION_ENABLE
					VisionTargeting->pidReset();
#endif
				}
			} else {
				driving->lightOff();
			}


			//if (rightJoy->GetRawButton(1) || leftJoy->GetRawButton(1)||rightJoy->GetRawButton(1) || leftJoy->GetRawButton(1)   ) {

		}
	}
	void TestPeriodic() {
		//lw->Run();
	}

};

START_ROBOT_CLASS(Robot);
