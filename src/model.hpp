#ifndef MODEL_HPP
#define MODEL_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "calibrator.hpp"
#include "desktopdrawer.hpp"

class Model {
private:
	IplImage* blackBoardImage;
	Calibrator* calibrator;
	DesktopDrawer* desktopDrawer;
	View *view;

public:
	static const int blackBoardWidth = 800;
	static const int blackBoardHeight = 600;
	CvCapture* webcam;
	int update();
	//actions on model
	void setTool();
	void clear();
	int calibrate();
	
	IplImage* getBlackBoardImage();
	void setBlackBoardImage(IplImage* img);
	char blackBoardWindow[64], webcamWindow[64];
	void init();
	Model(View *parent);
	~Model();
};

#endif
