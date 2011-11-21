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

	int W = frame->widthStep;
	int H = frame->height;
	int x, y;
	for (y = 0; y < H; y++)
		memset(maxSquare[y], 0, W / 6);

	int maxX, maxY, maxVal;
	maxX = maxY = maxVal = 0;
	for (y = 1; y < H; y++)
		for (x = 3; x < W; x += 3) {
			int R = (unsigned char) frame->imageData[y * W + x + 2];
			int G = (unsigned char) frame->imageData[y * W + x + 1];
			int B = (unsigned char) frame->imageData[y * W + x];
			if (isLightPen(R, G, B)) {
				/*maxSquare[y][x/3]=min(maxSquare[y-1][x/3],min(maxSquare[y][x/3-1],maxSquare[y-1][x/3-1]))+1;
				 if(maxSquare[y][x/3]>maxVal){
				 maxVal=maxSquare[y][x/3];
				 maxX=x/3;
				 maxY=y;
				 }*/
				CvPoint pixel = getDesktopCoords(x / 3, y);
				drawPoint(pixel);
			} else
				maxSquare[y][x / 3] = 0;
		}
	/*if(maxVal>0){
	 CvPoint pixel=getDesktopCoords(maxX-maxVal/2,maxY-maxVal/2);
	 if(lastPoint.x>0)
	 drawLine(lastPoint,pixel);
	 else
	 drawPoint(pixel);
	 lastPoint=pixel;
	 cerr << "mam pixel " << pixel.x << " " << pixel.y << endl;
	 } else {
	 lastPoint=cvPoint(-1,-1);
	 cerr << "nemam pixel\n";
	 }*/

	IplImage* resizedFrame = cvCreateImage(cvSize(320, 240), frame->depth,
			frame->nChannels);
	cvResize(frame, resizedFrame);
	cvShowImage(blackBoard->webcamWindow, resizedFrame);
	cvReleaseImage(&resizedFrame);
	cvShowImage(blackBoard->blackBoardWindow, desktop);

	return 1;
}

CvPoint DesktopDrawer::getDesktopCoords(int x, int y) {
	//binary search coordinates of webcam point (x,y) in rectangle in blackBoard->calibrator->calibrationData
	CalibrationData calibrationData = blackBoard->calibrator->calibrationData;
	const double epsilon = 0.005;
	double l, r, p;
	CvPoint A, B, C = cvPoint(x, y);
	CvPoint result;
	//x coordinate
	l = 0;
	r = 1;
	while (r - l > epsilon) {
		p = (l + r) / 2;
		A = cvPoint(calibrationData.vertex[0].x + p
				* (calibrationData.vertex[1].x - calibrationData.vertex[0].x),
				calibrationData.vertex[0].y + p * (calibrationData.vertex[1].y
						- calibrationData.vertex[0].y));
		B = cvPoint(calibrationData.vertex[3].x + p
				* (calibrationData.vertex[2].x - calibrationData.vertex[3].x),
				calibrationData.vertex[3].y + p * (calibrationData.vertex[2].y
						- calibrationData.vertex[3].y));
		if ((B.x - A.x) * (C.y - B.y) - (B.y - A.y) * (C.x - B.x) > 0)
			r = p;
		else
			l = p;
	}
	result.x = desktopWidth * l;
	//y coordinate
	l = 0;
	r = 1;
	while (r - l > epsilon) {
		p = (l + r) / 2;
		A = cvPoint(calibrationData.vertex[3].x + p
				* (calibrationData.vertex[0].x - calibrationData.vertex[3].x),
				calibrationData.vertex[3].y + p * (calibrationData.vertex[0].y
						- calibrationData.vertex[3].y));
		B = cvPoint(calibrationData.vertex[2].x + p
				* (calibrationData.vertex[1].x - calibrationData.vertex[2].x),
				calibrationData.vertex[2].y + p * (calibrationData.vertex[1].y
						- calibrationData.vertex[2].y));
		if ((B.x - A.x) * (C.y - B.y) - (B.y - A.y) * (C.x - B.x) > 0)
			r = p;
		else
			l = p;
	}
	result.y = desktopHeight * (1 - l);
	//cerr << "input: [" << C.x << ", " << C.y << "]  output: [" << result.x << ", " << result.y << "]\n";
	return result;
}

inline bool DesktopDrawer::isLightPen(int R, int G, int B) {
	return ((R > 220) && (R > (G + B) * 2 / 3));
}

inline void DesktopDrawer::drawPoint(CvPoint A) {
	desktop->imageData[A.y * desktop->widthStep + 3 * A.x + 2] = 0;
	desktop->imageData[A.y * desktop->widthStep + 3 * A.x + 1] = 255;
	desktop->imageData[A.y * desktop->widthStep + 3 * A.x + 0] = 255;
}

inline int sqr(int x) {
	return x * x;
}

void DesktopDrawer::drawLine(CvPoint A, CvPoint B) {
	int d = (int) (sqrt(sqr(A.x - B.x) + sqr(A.y - B.y) + 1));
	for (int i = 0; i <= d; i++) {
		drawPoint(cvPoint(A.x + i * (B.x - A.x) / d, A.y + i * (B.y - A.y) / d));
	}

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

	lastPoint = cvPoint(-1, -1);
}

DesktopDrawer::DesktopDrawer(BlackBoard* parent) {
	blackBoard = parent;
	desktopWidth = 800;
	desktopHeight = 600;
	desktop = cvCreateImage(cvSize(desktopWidth, desktopHeight), IPL_DEPTH_8U,
			3);

	clear();

	//initialization array for finding maximal square of lightPen pixels
	int captureWidth = cvGetCaptureProperty(blackBoard->webcam,
			CV_CAP_PROP_FRAME_WIDTH);
	int captureHeight = cvGetCaptureProperty(blackBoard->webcam,
			CV_CAP_PROP_FRAME_HEIGHT);
	cerr << "capture resolution is " << captureWidth << "x" << captureHeight
			<< endl;
	for (int i = 0; i < captureHeight; i++)
		maxSquare[i] = (int*) malloc(sizeof(int) * (captureWidth + 4));
}

DesktopDrawer::~DesktopDrawer() {
	int captureHeight = cvGetCaptureProperty(blackBoard->webcam,
			CV_CAP_PROP_FRAME_HEIGHT);
	for (int i = 0; i < captureHeight; i++)
		free( maxSquare[i]);

	cvReleaseImage(&desktop);
}
