#include "Vision41.h"
#include "WPILib.h"
#include <algorithm>
Vision41::Vision41()
//even if we are setting camera code elsewhere this is an og failsafe
{
	//aim is .11
	//back is .12
	camera = new AxisCamera("10.0.41.11");//axis-camera.local
	camera->WriteBrightness(30);
	camera->WriteColorLevel(100);
	camera->WriteWhiteBalance(camera->kWhiteBalance_FixedOutdoor2);
	camera->WriteMaxFPS(25);
	camera->WriteResolution(AxisCamera::kResolution_320x240);
	//lightTalon = new Talon(9);
	centerMassXNormalized = 0;

}

double scoreRectangularity(ParticleAnalysisReport *report){
	if(report->boundingRect.width*report->boundingRect.height !=0){
		return 100*report->particleArea/(report->boundingRect.width*report->boundingRect.height);
	}
	else{
		return 0;
	}
}

double ratioToScore(double ratio){
	int ans1 = 100*(1-fabs(1-ratio));
	int minimum = std::min(ans1, 100);
	return (std::max((int)0, minimum));
}

double scoreAspectRatio(BinaryImage *image, ParticleAnalysisReport *report){
	double rectLong, rectShort, idealAspectRatio, aspectRatio;
	idealAspectRatio = 20.0/12.0;  //Vertical Reflector 4" wide x 32" tall, horizontal 23.5" wide x 4" tall

	imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &rectLong);
	imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &rectShort);

	//Divide width by height to measure aspect ratio
	if(report->boundingRect.width > report->boundingRect.height)
		aspectRatio = ratioToScore(((rectLong/rectShort)/idealAspectRatio));

	return aspectRatio;
}

double getWidth(ParticleAnalysisReport *report){
	return report->boundingRect.width;
}

double computeDistance (BinaryImage *image, ParticleAnalysisReport *report){
	double rectLong, height;
	int targetHeight;

	imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &rectLong);

	height = std::min((double)report->boundingRect.height, rectLong);
	targetHeight = 12;

	return Y_IMAGE_RES *targetHeight/ (height *2 *tan(VIEW_ANGLE*PI/(180*2)));
}

bool scoreCompare(Scores scores){
	return (scores.rectangularity > RECTANGULARITY_LIMIT && scores.aspectRatio > ASPECT_RATIO_LIMIT);
}
//Can we change Cam Settings at will?
void Vision41::AutoCam()
{
	camera->WriteBrightness(30);
	camera->WriteColorLevel(100);
	camera->WriteWhiteBalance(camera->kWhiteBalance_FixedOutdoor2);
	camera->WriteMaxFPS(25);
	camera->WriteResolution(AxisCamera::kResolution_320x240);
	printf("Autocam Lives On");
}
void Vision41::TeleCam()
{
	camera->WriteBrightness(50);
	camera->WriteColorLevel(50);
	camera->WriteWhiteBalance(camera->kWhiteBalance_Automatic);
	//camera->WriteCompression(30);
	printf("TELECAM HAS ACTIVATED");

}
void Vision41::AutoAim()
{
	//printf("%i",camera==0);
	if (camera->IsFreshImage())
	{
		//printf("Fresh Image\n");
		ColorImage * image = camera->GetImage();
#ifdef IMAGEFILEOUT_ENABLE
		image->Write("/home/lvuser/original.png");
#endif
		//tempOriginalImage = image;
		//BinaryImage *thresholdImage = image->ThresholdRGB(0, 202, 98, 255, 102, 255); //Pit Cam
		//BinaryImage *thresholdImage = image->ThresholdRGB(0, 155, 119, 255, 45, 255); // GOODER ONES FOR NATS BOIS
//		BinaryImage *thresholdImage = image->ThresholdRGB(0, 88, 40, 150, 0, 255); //final nats ?????  possibly
		BinaryImage *thresholdImage = image->ThresholdRGB(0, 69, 31, 255, 0,93);// brunswick eruption
//		BinaryImage *thresholdImage = image->ThresholdRGB(0, 124, 83, 255, 0,1119);// pittest

		/*       filters out ambient lighting.

			Axis camera settings:
				Color Level : 100
				Brightness  : 30
				Sharpness   : 50
				Contrast    : 9
				White Balance : Fixed Outdoor 2
				Exposure Value : 10

		*/
		//BinaryImage *thresholdImage = image->ThresholdHSL(80, 150, 0, 255, 150, 200);
#ifdef IMAGEFILEOUT_ENABLE
		thresholdImage->Write("/home/lvuser/threshold.png");
#endif
		BinaryImage *convexHullImage = thresholdImage->ConvexHull(false);
#ifdef IMAGEFILEOUT_ENABLE
		convexHullImage->Write("/home/lvuser/convexHull.png");
#endif
		ParticleFilterCriteria2 criteria[] = {{IMAQ_MT_AREA, AREA_MINIMUM, 65535, false, false}};
		BinaryImage *filteredImage = convexHullImage->ParticleFilter(criteria,1);
#ifdef IMAGEFILEOUT_ENABLE
		filteredImage->Write("/home/lvuser/filtered.png");
#endif
		std::vector<ParticleAnalysisReport> *reports = filteredImage->GetOrderedParticleAnalysisReports();

		scores = new Scores[reports->size()];

		//printf("Number of Particles: %d\n",reports->size());

		double targetsCmxn[5];
		double targetsWidth[5];
		double targetsRatio[5];

		numTargets = 0;
		centerMassXNormalized = 0;
		ParticleAnalysisReport *targetReport;
		for(unsigned i=0;i<reports->size();i++)
		{
			ParticleAnalysisReport *report;
			report = &(reports->at(i));
			//printf("Particle %d", i);
			scores[i].rectangularity = scoreRectangularity(report);
			//printf("Rectangularity: %f\n",scores[i].rectangularity);
			scores[i].aspectRatio = scoreAspectRatio(filteredImage, report);
			//printf("AspectRatio: %f\n", scores[i].aspectRatio);

			//printf("Normalized center of x mass: %f\n",report->center_mass_x_normalized);
			if(scoreCompare(scores[i]))
			{
				numTargets++;
				targetReport = &(reports->at(i));
				centerMassXNormalized = targetReport->center_mass_x_normalized;
				if(i < 5)
				{
					targetsCmxn[i] = centerMassXNormalized;
					targetsWidth[i] = getWidth(report);
					targetsRatio[i] = scores[i].aspectRatio;
				}
				//printf("DISTANCE: %f\n",computeDistance(thresholdImage,report));
				SmartDashboard::PutNumber("number of targets: ",numTargets);
				//printf("I see %d targets\n",numTargets);
			}
		}
		if(numTargets==0)
		{
			//pidReset();
		}

		double widest = 0;
		double wideCmxn = 0;
		//returns the wider goal
		for(int i=0;i<5;i++)
		{
			if(targetsWidth[i] > widest)
			{
				wideCmxn = targetsCmxn[i];
				widest = targetsWidth[i];
				currentGoal = targetsRatio[i];
			}
		}
		//if(centerMassXNormalized != 0){
			//centerMassXNormalized = wideCmxn;
		if(numTargets!=0)
			pid();

			//printf("working");
		//}
		//printf("DISTANCE: %f\n",computeDistance(thresholdImage,targetReport));
		delete image;
		delete thresholdImage;
		delete convexHullImage;
		delete filteredImage;
	}
	//else
		//printf("not new image");

}



void Vision41::pid(){
	error = centerMassXNormalized - target;
	error*=-1;
	//double ki = (0.0054/(error*error));
	//double ki = 0.05;
	//printf("error: ",(double)error);

	//if(fabs(error) < 0.2 || accumulated * error < 0)

			accumulated += error;

	if(accumulated > maxAccumulated)
		accumulated = maxAccumulated;

	if(accumulated < -maxAccumulated)
		accumulated = -maxAccumulated;
	//printf("%f",accumulated);




	out = K_P*error + K_I*accumulated + K_D*(-prevError + error);
	//printf("p=%f i=%f d=%f\n", error*K_P, accumulated*K_I, (-prevError+error)*K_D);
	//printf(" output%f\n ", out);
	printf("Error%f\n", error);
	//printf("Ki stuff ",K_I*accumulated);
	//printf("K_P:",K_P*error);
	//printf("integral:",K_I*accumulated);
	//printf("derivative:",K_D*(prevError - error));
	// Maybe change the hard limits we have into a multiple of error?

	prevError = error;
}

void Vision41::pidReset(){
	accumulated = 0;
	error = 0;
	prevError =0;

}

void Vision41::aspectTarget()
{
	//perfect goal ratio = 1.66666666666667
	goalOut = (currentGoal/perfectGoal)*goalRange;

	if(centerMassXNormalized < goalOut)
		onTar = true;
	else
		onTar = false;
	SmartDashboard::PutBoolean("On Target:",onTar);
}
/*
void Vision41::AutoAim()
{
	//TODO: can be revmoved if everywhere is replaced by get image
	printf("PLZ\n");
	getImage();
	printf("hi\n");

}
*/



