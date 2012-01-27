#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cstring>
#include <cmath>
#include "pathfinder_all_red.hpp"
#include "pathfinder.hpp"
#include "pathfinder.cpp"

using namespace std;

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
