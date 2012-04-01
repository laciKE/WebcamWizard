#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <cmath>
#include "blackboard.hpp"
#include "desktopdrawer.hpp"
#include "calibrator.hpp"

using namespace std;

//update desktop from blackboard due to captured frame from webcam
//result will be stored in desktop
//return 1 iff success, otherwise 0
//(Laco)
int DesktopDrawer::update() {
	IplImage* frame = cvQueryFrame(blackBoard->webcam);
	if (!frame)
		return 0;

	IplImage* resizedFrame = cvCreateImage(cvSize(320, 240), frame->depth,
			frame->nChannels);
	cvResize(frame, resizedFrame);
	cvShowImage(blackBoard->webcamWindow, resizedFrame);
	cvReleaseImage(&resizedFrame);

	if (!pathFinder)
		return 0;

	pathFinder->drawPath(frame, desktop);

	cvShowImage(blackBoard->blackBoardWindow, desktop);

	return 1;
}

void DesktopDrawer::clear() {
	int W = desktop->widthStep;
	int H = desktop->height;
	int x, y;
	for (y = 0; y < H; y++)
		for (x = 0; x < W; x += 3) {
			desktop->imageData[y * W + x + 2] = 32;
			desktop->imageData[y * W + x + 1] = 32;
			desktop->imageData[y * W + x] = 32;
		}
}

DesktopDrawer::DesktopDrawer(BlackBoard* parent) {
	blackBoard = parent;
	desktopWidth = parent->blackBoardWidth;
	desktopHeight = parent->blackBoardHeight;
	desktop = parent->blackBoardImage;

	clear();

	//initialization array for finding maximal square of lightPen pixels
	int captureWidth = cvGetCaptureProperty(blackBoard->webcam,
			CV_CAP_PROP_FRAME_WIDTH);
	int captureHeight = cvGetCaptureProperty(blackBoard->webcam,
			CV_CAP_PROP_FRAME_HEIGHT);
	cerr << "capture resolution is " << captureWidth << "x" << captureHeight
			<< endl;
}

void DesktopDrawer::setPathFinder(class PathFinder* pF){
	pathFinder = pF;
	pathFinder->Init();
}

DesktopDrawer::~DesktopDrawer() {
	int captureHeight = cvGetCaptureProperty(blackBoard->webcam,
			CV_CAP_PROP_FRAME_HEIGHT);

	cvReleaseImage(&desktop);
}
