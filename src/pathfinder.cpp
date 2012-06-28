#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "pathfinder.hpp"
#include "model.hpp"

using namespace std;

CvPoint PathFinder::getDesktopCoords(int x, int y) {
	//binary search coordinates of webcam point (x,y) in rectangle in blackBoard->calibrator->calibrationData
	CalibrationData calibrationData = model->calibrator->calibrationData;
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
	result.x = model->blackBoardWidth * l;
	//cerr << l << " ";
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
	result.y = model->blackBoardHeight * (1 - l);
	//cerr << 1-l << endl;
	//cerr << "input: [" << C.x << ", " << C.y << "]  output: [" << result.x << ", " << result.y << "]\n";
	return result;
}

inline bool PathFinder::isLightPen(int R, int G, int B) {
        //return ((R > 220) && (R > (G + B) * 2 / 3));
        return ((B > 220) && (B > (G + R) * 2 / 3));
}

inline void PathFinder::drawPoint(CvPoint A, IplImage* img) {
	img->imageData[(int)(A.y * img->widthStep + 3 * A.x + 2)] = 0;
	img->imageData[(int)(A.y * img->widthStep + 3 * A.x + 1)] = 255;
	img->imageData[(int)(A.y * img->widthStep + 3 * A.x + 0)] = 255;
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

PathFinder::PathFinder(Model *m){
	model = m;
}

PathFinder::~PathFinder(){

}




//Find and draw all red pixels from frame

void PathFinderAllRed::Init() {
	model->debug("PathFinderAllRed");
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

PathFinderAllRed::PathFinderAllRed(Model *m):PathFinder(m){
}

PathFinderAllRed::~PathFinderAllRed(){
}



//Find max lightpen square in frame and draw line with endpoints in center of two squares

void PathFinderMaxSquare::Init() {
	lastPoint.x = -1;
	lastPoint.y = -1;
	model->debug("PathFinderMaxSquare");
}

void PathFinderMaxSquare::drawPath(IplImage* frame, IplImage* desktop){
	int W = frame->widthStep;
	int H = frame->height;
	int x, y;
	for (y = 0; y < H; y++)
		memset(maxSquare[y], 0, W / 3);

	int maxX, maxY, maxVal;
	maxX = maxY = maxVal = 0;
	for (y = 1; y < H; y++)
		for (x = 3; x < W; x += 3) {
			int R = (unsigned char) frame->imageData[y * W + x + 2];
			int G = (unsigned char) frame->imageData[y * W + x + 1];
			int B = (unsigned char) frame->imageData[y * W + x];
			if (isLightPen(R, G, B)) {
				maxSquare[y][x/3]=min(maxSquare[y-1][x/3],min(maxSquare[y][x/3-1],maxSquare[y-1][x/3-1]))+1;
				if(maxSquare[y][x/3]>maxVal){
					maxVal=maxSquare[y][x/3];
					maxX=x/3;
					maxY=y;
			 	}
			} else
				maxSquare[y][x / 3] = 0;
		}
	if(maxVal>1){
		CvPoint pixel=getDesktopCoords(maxX-maxVal/2,maxY-maxVal/2);
		if(lastPoint.x>0)
			drawLine(lastPoint,pixel,desktop);
		else
			drawPoint(pixel,desktop);
		lastPoint=pixel;
//		cerr << "mam pixel " << pixel.x << " " << pixel.y << endl;
	 } else {
	 	lastPoint=cvPoint(-1,-1);
//	 	cerr << "nemam pixel\n";
	 }
}

PathFinderMaxSquare::PathFinderMaxSquare(Model *m):PathFinder(m){
	//initialization array for finding maximal square of lightPen pixels
	int captureWidth = cvGetCaptureProperty(model->webcam,
			CV_CAP_PROP_FRAME_WIDTH);
	int captureHeight = cvGetCaptureProperty(model->webcam,
			CV_CAP_PROP_FRAME_HEIGHT);
	if(!captureWidth){
		captureWidth=640;
		captureHeight=480;
	}
	//cerr << "capture resolution is " << captureWidth << "x" << captureHeight << endl;
	maxSquare = (int**) malloc(sizeof(int*) * (captureHeight + 4));
	for (int i = 0; i < captureHeight; i++)
		maxSquare[i] = (int*) malloc(sizeof(int) * (captureWidth + 4));
}

PathFinderMaxSquare::~PathFinderMaxSquare(){
	int captureHeight = cvGetCaptureProperty(model->webcam,
			CV_CAP_PROP_FRAME_HEIGHT);
	for (int i = 0; i < captureHeight; i++)
		free(maxSquare[i]);
	free(maxSquare);
}



//Fit lightpen pixels in frame with linea and draw it, connected to previous line, if possible

void PathFinderFitLine::Init() {
	lastPoint.x = -1;
	lastPoint.y = -1;

	cvClearSeq(point_seq);

	model->debug("PathFinderFitLine");
}
/*
	CvPoint2D32f points[10];
	for(int i=0; i<10; i++){
		points[i].x=points[i].y=i;
	}
	float line[4];
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* point_seq = cvCreateSeq( CV_32FC2, sizeof(CvSeq), sizeof(CvPoint2D32f), storage );
	for(int i=0; i<10; i++)
		cvSeqPush(point_seq, &points[i]);
	cvFitLine(point_seq,CV_DIST_L2,0,0.01,0.01,line);
	for(int i=0; i<4; i++)
		cerr << line[i] << endl;
*/
void PathFinderFitLine::drawPath(IplImage* frame, IplImage* desktop){
	int W = frame->widthStep;
	//int H = frame->height;
	int x, y, numOfPoints=0;
	cvClearSeq(point_seq);
	float line[4];
	vector <CvPoint2D32f> points;

	for (y = min(model->calibrator->calibrationData.vertex[0].y,model->calibrator->calibrationData.vertex[1].y); y < max(model->calibrator->calibrationData.vertex[2].y,model->calibrator->calibrationData.vertex[3].y); y++)
		for (x = 3*min(model->calibrator->calibrationData.vertex[0].x,model->calibrator->calibrationData.vertex[1].x); x < 3*max(model->calibrator->calibrationData.vertex[2].x,model->calibrator->calibrationData.vertex[3].x); x += 3) {
			int R = (unsigned char) frame->imageData[y * W + x + 2];
			int G = (unsigned char) frame->imageData[y * W + x + 1];
			int B = (unsigned char) frame->imageData[y * W + x];
			if (isLightPen(R, G, B)){
				points.push_back(cvPoint2D32f(x/3,y));
				cvSeqPush(point_seq,&points[numOfPoints++]);
			}
		}

	if(numOfPoints>3){
		cvFitLine(point_seq,CV_DIST_L2,0,0.1,0.1,line);
		//for(int i=0; i<4; i++)
		//	cerr << line[i] << " ";

		CvPoint pixel=getDesktopCoords(line[2],line[3]);
		if(lastPoint.x>0)
			drawLine(lastPoint,pixel,desktop);
		else
			drawPoint(pixel,desktop);
		lastPoint=pixel;
		//cerr << "mam pixel " << pixel.x << " " << pixel.y << endl;
	 } else {
	 	lastPoint=cvPoint(-1,-1);
	 	//cerr << "nemam pixel\n";
	}
}

PathFinderFitLine::PathFinderFitLine(Model *m):PathFinder(m){
	storage = cvCreateMemStorage(0);
	point_seq = cvCreateSeq( CV_32FC2, sizeof(CvSeq), sizeof(CvPoint2D32f), storage );
}

PathFinderFitLine::~PathFinderFitLine(){
	cvClearSeq(point_seq);
	cvReleaseMemStorage(&storage);
}
