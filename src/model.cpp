#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <cstring>
#include "model.hpp"

using namespace std;

int Model::update() {
	return desktopDrawer->update();
}

void Model::setTool(){

}

void Model::clear(){
	desktopDrawer->clear();
}

IplImage* Model::getBlackBoardImage(){
	return cvCloneImage(blackBoardImage);
}

void Model::setBlackBoardImage(IplImage *img ){
	cvReleaseImage(&blackBoardImage);
	blackBoardImage = cvCloneImage(img);
	view->refresh();
}

Model::~Model() {
	delete calibrator;
	delete desktopDrawer;
	cvReleaseCapture(&webcam);
	cvDestroyWindow(blackBoardWindow);
	cvDestroyWindow(webcamWindow);
}

Model::Model(View *parent) {
	view = parent;
	//window names initialization
	strcpy(blackBoardWindow, "Model");
	strcpy(webcamWindow, "Webcam");
	//webcam initialization
	webcam = cvCaptureFromCAM(0);
	//webcam=cvCreateCameraCapture(CV_CAP_ANY);
	if (!webcam) {
		cerr << "Create webcam capture failed\n";
		exit(1);
	}
	blackBoardImage = cvCreateImage(cvSize(blackBoardWidth, blackBoardHeight), IPL_DEPTH_8U,3);
	//private variables initialization
	desktopDrawer = new DesktopDrawer(this);
	calibrator = new Calibrator(this);
}

void Model::init() {
	//windows initialization
	cvNamedWindow(webcamWindow, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(webcamWindow, 600, 0);
	
	cvNamedWindow(blackBoardWindow, CV_WINDOW_AUTOSIZE);
	//cvMoveWindow(blackBoardWindow, 328, 0);
	cvMoveWindow(blackBoardWindow, 0, 0);

	//cvSetWindowProperty("Model",CV_WND_PROP_FULLSCREEN,0);

	//calibrator->calibrate();
}

int Model::calibrate(){
	return calibrator->calibrate();
}
