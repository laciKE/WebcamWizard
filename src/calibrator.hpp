#ifndef CALIBRATOR_HPP
#define CALIBRATOR_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "model.hpp"

typedef struct {
        CvPoint vertex[4];
} CalibrationData;

class Calibrator {
private:
	class Model* model;
	IplImage* DetectAndDrawQuads(IplImage* img);

public:
        //coordinates of blackboard's corners in webcam
        CalibrationData calibrationData;
        Calibrator(Model* parent);
        ~Calibrator();
        int calibrate();
};

#endif
