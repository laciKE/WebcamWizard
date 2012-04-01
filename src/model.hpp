#ifndef MODEL_HPP
#define MODEL_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "calibrator.hpp"
#include "desktopdrawer.hpp"

class Model {
private:
	IplImage* blackBoardImage;

public:
	static const int blackBoardWidth = 800;
	static const int blackBoardHeight = 600;
	CvCapture* webcam;
	Calibrator* calibrator;
	DesktopDrawer* desktopDrawer;
	int update();
	//actions on model
	void setTool();
	void clear();
	
	IplImage* getBlackBoardImage();
	void setBlackBoardImage(IplImage* img);
	char blackBoardWindow[64], webcamWindow[64];
	void init();
	Model();
	~Model();
};

#endif
