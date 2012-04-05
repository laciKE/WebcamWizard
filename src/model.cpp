#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <cstring>
#include "model.hpp"

using namespace std;

int Model::update() {
	IplImage *frame = cvQueryFrame(webcam);
	if (!frame)
		return 0;

	//TODO moznost vypnut zobrazovanie webcamImage
	cvResize(frame, webcamImage);
	cvShowImage(webcamWindow, webcamImage);

	if (!pathFinder)
		return 0;

	pathFinder->drawPath(frame, blackBoardImage);

	cvShowImage(blackBoardWindow, blackBoardImage);

	return 1;
}

void Model::setTool(){

}

void Model::clear(){
	int W = blackBoardImage->widthStep;
	int H = blackBoardImage->height;
	int x, y;
	for (y = 0; y < H; y++)
		for (x = 0; x < W; x += 3) {
			blackBoardImage->imageData[y * W + x + 2] = 32;
			blackBoardImage->imageData[y * W + x + 1] = 32;
			blackBoardImage->imageData[y * W + x] = 32;
		}
}

IplImage* Model::getBlackBoardImage(){
	return cvCloneImage(blackBoardImage);
}

IplImage* Model::getWebcamImage(){
	return cvCloneImage(webcamImage);
}

void Model::setBlackBoardImage(IplImage *img ){
	if((img->width != blackBoardWidth) || (img->height != blackBoardHeight)){
		cerr << "bad image size in model::setBlackBoardImage(IplImage *img)\n";
		return;
	}
	cvReleaseImage(&blackBoardImage);
	blackBoardImage = cvCloneImage(img);
	view->refresh();
}

void Model::setPathFinder(class PathFinder* pF){
	pathFinder = pF;
	pathFinder->Init();
}

Model::~Model() {
	delete calibrator;
	//delete desktopDrawer;
	cvReleaseCapture(&webcam);
	cvReleaseImage(&blackBoardImage);
	cvReleaseImage(&webcamImage);
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

	//create webcamImage
	IplImage *frame = cvQueryFrame(webcam);
	if (!frame) {
		cerr << "Query frame failed\n";
		exit(1);
	}

	webcamImage = cvCreateImage(cvSize(320, 240), frame->depth,
			frame->nChannels);


	//private variables initialization
	//desktopDrawer = new DesktopDrawer(this);
	calibrator = new Calibrator(this);
}

void Model::Init() {
	//windows initialization
	cvNamedWindow(webcamWindow, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(webcamWindow, 600, 0);
	
	cvNamedWindow(blackBoardWindow, CV_WINDOW_AUTOSIZE);
	//cvMoveWindow(blackBoardWindow, 328, 0);
	cvMoveWindow(blackBoardWindow, 0, 0);

	clear();

	//cvSetWindowProperty("Model",CV_WND_PROP_FULLSCREEN,0);

	//calibrator->calibrate();
}

int Model::calibrate(){
	int result = calibrator->calibrate();
	setBlackBoardImage(cvCreateImage(cvSize(blackBoardWidth, blackBoardHeight), IPL_DEPTH_8U,3));
	clear();
	return result;
}
