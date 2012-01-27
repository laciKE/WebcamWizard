#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "pathfinder.hpp"
#include "blackboard.hpp"

using namespace std;

CvPoint PathFinder::getDesktopCoords(int x, int y) {
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
	result.x = blackBoard->desktopDrawer->desktopWidth * l;
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
	result.y = blackBoard->desktopDrawer->desktopHeight * (1 - l);
	//cerr << "input: [" << C.x << ", " << C.y << "]  output: [" << result.x << ", " << result.y << "]\n";
	return result;
}

inline bool PathFinder::isLightPen(int R, int G, int B) {
	return ((R > 220) && (R > (G + B) * 2 / 3));
}

inline void PathFinder::drawPoint(CvPoint A, IplImage* img) {
	img->imageData[A.y * img->widthStep + 3 * A.x + 2] = 0;
	img->imageData[A.y * img->widthStep + 3 * A.x + 1] = 255;
	img->imageData[A.y * img->widthStep + 3 * A.x + 0] = 255;
}

inline int sqr(int x) {
	return x * x;
}

void PathFinder::drawLine(CvPoint A, CvPoint B, IplImage* img) {
	int d = (int) (sqrt(sqr(A.x - B.x) + sqr(A.y - B.y) + 1));
	for (int i = 0; i <= d; i++) {
		drawPoint(cvPoint(A.x + i * (B.x - A.x) / d, A.y + i * (B.y - A.y) / d), img);
	}

}

PathFinder::PathFinder(BlackBoard* bB){
	blackBoard = bB;
}

PathFinder::~PathFinder(){

}




//Find and draw all red pixels from frame

void PathFinderAllRed::Init() {
}

void PathFinderAllRed::drawPath(IplImage* frame, IplImage* desktop){
	int W = frame->widthStep;
	int H = frame->height;
	int x, y;
	for (y = 1; y < H; y++)
		for (x = 3; x < W; x += 3) {
			int R = (unsigned char) frame->imageData[y * W + x + 2];
			int G = (unsigned char) frame->imageData[y * W + x + 1];
			int B = (unsigned char) frame->imageData[y * W + x];
			if (isLightPen(R, G, B)) {
				CvPoint pixel = getDesktopCoords(x / 3, y);
				drawPoint(pixel,desktop);
			}
		}
}

PathFinderAllRed::PathFinderAllRed(BlackBoard* bB):PathFinder(bB){
}

PathFinderAllRed::~PathFinderAllRed(){
}
