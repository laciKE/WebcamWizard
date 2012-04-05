#ifndef MODEL_HPP
#define MODEL_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "calibrator.hpp"
#include "pathfinder.hpp"
#include "view.hpp"

class Model {
private:
	IplImage *blackBoardImage;
	IplImage *webcamImage;
	class View *view;
	class PathFinder *pathFinder;

public:
	class Calibrator *calibrator; //TODO getCalibrationData
	static const int blackBoardWidth = 800;
	static const int blackBoardHeight = 600;
	CvCapture *webcam;
	int update();
	//actions on model
	void setTool();
	void setPathFinder(PathFinder *pF);
	void clear();
	int calibrate();
	
	IplImage *getBlackBoardImage();
	IplImage *getWebcamImage();
	void setBlackBoardImage(IplImage *img);
	char blackBoardWindow[64], webcamWindow[64];
	void Init();
	Model(View *parent);
	//Model();
	~Model();
};

#endif
