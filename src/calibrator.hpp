#ifndef CALIBRATOR_HPP
#define CALIBRATOR_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "blackboard.hpp"

typedef struct
{
	CvPoint vertex[4];
} CalibrationData;


class Calibrator
{
private:
	class BlackBoard * blackBoard;

public:
	//coordinates of blackboard's corners in webcam
	CalibrationData calibrationData;
	Calibrator(BlackBoard * parent);
	~Calibrator();
	int calibrate();
};

#endif
