#ifndef MODEL_HPP
#define MODEL_HPP

#include <QApplication>
#include <QtGui>
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
	static int blackBoardWidth;
	static int blackBoardHeight;
	CvCapture *webcam;
	int update();
	//actions on model
	void setTool();
	void setPathFinder(PathFinder *pF);
	void clear();
	int calibrate();
	void debug(const char *str);
	
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
