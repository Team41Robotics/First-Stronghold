#include "WPILib.h"

#ifndef VISION41_H
#define VISION41_H
#define VISION_AUTO_ENABLE
#define VISION_ENABLE

//#define IMAGEFILEOUT_ENABLE	//uncomment for debug

#define RECTANGULARITY_LIMIT 30
#define ASPECT_RATIO_LIMIT 40
#define Y_IMAGE_RES 480
#define AREA_MINIMUM 150
#define VIEW_ANGLE 49//for the Axis M1013
#define PI 3.141592653
#define OFFSET 0.081250     //10625   //0.093750//0.11//-0.02 // corrects for camera being off center. -0.043750



struct Scores{
		double rectangularity;
		double aspectRatio;
		double xEdge;
		double yEdge;
	};

struct TargetReport{
		int verticalIndex;
		int horizontalIndex;
		bool Hot;
		double totalScore;
		double leftScore;
		double rightScore;
		double tapeWidthScore;
		double verticalScore;
	};

class Vision41
{
private:

	Scores *scores;
	/* not sleep deprived values
	 * KP=0.6925 ->
	 * KI=0.055 0.1 -> 0.6/0.8
	 * KD=0.03 ->
	 * max accu = 5 3
	 *
	 */
	double target = OFFSET;
	double prevError=0;
	double accumulated=0;
	double maxAccumulated = 8;
	//double K_C = 0.888; //0.888 at
	double K_P = .6;//0.780;//0.795//0.79;//0.79;//0.6925;//0.70;//0.97;//0.9;//0.6*K_C;//0.540;//0.670;     //0.6*Kc
	double K_I = 0.053;//0.072;//0.075;//0.14//0.5;//0.1;//0.06;//0.0003;//(2*K_P)/K_C;//0.13;//0.09;      //2*Kp/Kc
	double K_D = 0.34;//0.34;//0.33//0.3;//0.03;//0.04; //0.04;//0.125*K_P*1;//0.10;//0.11;    //0.125*Kp*Pc
	bool onTar = false;
	/*Values free from battery corruption that work very minimally
	kp 0.83-> 0.80 -> same -> same
	ki 0.00005 -> same -> 0.00006 -> same
	max accu 6600 -> 6700 -> 5700 -> 5800
	kd 0.03 -> 0.04 -> same -> same
	1.3
	*/

	/*Values cause everything sucks
	 kp 0.80
	 ki 0.00075
	 max accu 536
	 kd 0.04
	 */
	/*Values cause everything sucks
		 kp 0.70
		 ki 0.70
		 max accu 25
		 kd 0.04
		 */
	double currentGoal =0;
	double perfectGoal = 1.6666666667;
	double goalRange = 0.5;
	double goalOut = 0;
public:
	AxisCamera *camera;
	//Talon *lightTalon;
	double out;
	Vision41();
	void AutoCam();
	void TeleCam();
	void pid();
	void aspectTarget();
	void pidReset();
	void AutoAim();
	int numTargets = 0;

	//ColorImage *tempOriginalImage;
	//BinaryImage *tempThresholdImage;
	double centerMassXNormalized;
	double error=0;
};


#endif

