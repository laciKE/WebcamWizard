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
	CvScalar color;
	CvScalar bgColor;
	int thickness;
	const static int numberOfPathFinders = 3;
	class PathFinder *pathFinder[numberOfPathFinders];
	int tool;
public:
	class Calibrator *calibrator; //TODO getCalibrationData
	int blackBoardWidth;
	int blackBoardHeight;
	CvCapture *webcam;
	int update();
	//actions on model
	void setColor(const CvScalar& color);
	void setThickness(int thickness);
	void setTool(int tool);
	//void setPathFinder(PathFinder *pF);
	void clear();
	int calibrate();
	void debug(const char *str);
	
	IplImage *getBlackBoardImage();
	IplImage *getWebcamImage();
	void setBlackBoardImage(IplImage *img);
	char blackBoardWindow[64], webcamWindow[64];
	void registerView(View *view);
	void Init();
	//Model(View *parent);
	Model();
	~Model();
};

#endif
