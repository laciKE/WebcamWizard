#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <cstring>
#include "blackboard.hpp"

using namespace std;

int BlackBoard::update(){
	return desktopDrawer->update();
}

BlackBoard::~BlackBoard(){
	delete calibrator;
	delete desktopDrawer;
	cvReleaseCapture(&webcam);
	cvDestroyWindow(blackBoardWindow);
	cvDestroyWindow(webcamWindow);
}

BlackBoard::BlackBoard(){
	//windows initialization
	strcpy(blackBoardWindow,"BlackBoard");
	strcpy(webcamWindow,"Webcam");
	cvNamedWindow(blackBoardWindow, CV_WINDOW_AUTOSIZE );
	cvMoveWindow(blackBoardWindow,328,0);

	cvNamedWindow(webcamWindow,CV_WINDOW_AUTOSIZE);
	cvMoveWindow(webcamWindow,0,0);
	//cvSetWindowProperty("BlackBoard",CV_WND_PROP_FULLSCREEN,0);

	//webcam initialization 
	webcam=cvCaptureFromCAM(0);
	//webcam=cvCreateCameraCapture(CV_CAP_ANY); 
	if(!webcam){
		cerr << "Create webcam capture failed\n";
		exit(1);
	}
	//private variables initialization
	desktopDrawer = new DesktopDrawer(this);
	calibrator = new Calibrator(this);
	calibrator->calibrate();
}

