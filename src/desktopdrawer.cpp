#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <cstring>
#include "blackboard.hpp"
#include "desktopdrawer.hpp"
#include "calibrator.hpp"

using namespace std;

//update desktop from blackboard due to captured frame from webcam 
//result will be stored in desktop
//return 1 iff success, otherwise 0
//(Laco)
int DesktopDrawer::update(){
	IplImage * frame=cvQueryFrame(blackBoard->webcam);
	if(!frame) return 0;

	int W=frame->widthStep;
	int H=frame->height;
	int x,y;
	for(y=0; y<H/2; y++)
		memset(maxSquare[y],0,W/6);

	for(y=0; y<H; y++)
	for(x=0; x<W; x+=3) 
	{
		int R=(unsigned char)frame->imageData[y*W+x+2];
   		int G=(unsigned char)frame->imageData[y*W+x+1];
		int B=(unsigned char)frame->imageData[y*W+x];
		if(isLightPen(R,G,B)){
			//CvPoint pixel=cvPoint(0,0);
			CvPoint pixel=getDesktopCoords(x/3,y);
			desktop->imageData[pixel.y*desktop->widthStep+3*pixel.x+2]=0;
			desktop->imageData[pixel.y*desktop->widthStep+3*pixel.x+1]=255;
			desktop->imageData[pixel.y*desktop->widthStep+3*pixel.x+0]=0;
		}
	}

	IplImage * resizedFrame=cvCreateImage(cvSize(320, 240), frame->depth, frame->nChannels );
	cvResize(frame,resizedFrame);
	cvShowImage(blackBoard->webcamWindow,resizedFrame);
	cvReleaseImage(&resizedFrame);
	cvShowImage(blackBoard->blackBoardWindow,desktop);

	return 1;	
}

CvPoint DesktopDrawer::getDesktopCoords(int x, int y){
	//binary search coordinates of webcam point (x,y) in rectangle in blackBoard->calibrator->calibrationData
	CalibrationData calibrationData=blackBoard->calibrator->calibrationData;
	double l, r, p;
	CvPoint A,B,C=cvPoint(x,y);
	CvPoint result;
	//x coordinate
	l=0; r=1;
	while(r-l>0.001){
		p=(l+r)/2;
		A=cvPoint(calibrationData.vertex[0].x+p*(calibrationData.vertex[1].x-calibrationData.vertex[0].x),
			calibrationData.vertex[0].y+p*(calibrationData.vertex[1].y-calibrationData.vertex[0].y));
		B=cvPoint(calibrationData.vertex[3].x+p*(calibrationData.vertex[2].x-calibrationData.vertex[3].x),
			calibrationData.vertex[3].y+p*(calibrationData.vertex[2].y-calibrationData.vertex[3].y));
		if((B.x-A.x)*(C.y-B.y)-(B.y-A.y)*(C.x-B.x)>0)
			r=p;
		else
			l=p;
	}
	result.x=desktopWidth*l;
	//y coordinate
	l=0; r=1;
	while(r-l>0.001){
		p=(l+r)/2;
		A=cvPoint(calibrationData.vertex[3].x+p*(calibrationData.vertex[0].x-calibrationData.vertex[3].x),
			calibrationData.vertex[3].y+p*(calibrationData.vertex[0].y-calibrationData.vertex[3].y));
		B=cvPoint(calibrationData.vertex[2].x+p*(calibrationData.vertex[1].x-calibrationData.vertex[2].x),
			calibrationData.vertex[2].y+p*(calibrationData.vertex[1].y-calibrationData.vertex[2].y));
		if((B.x-A.x)*(C.y-B.y)-(B.y-A.y)*(C.x-B.x)>0)
			r=p;
		else
			l=p;
	}
	result.y=desktopHeight*(1-l);
cerr << "input: [" << C.x << ", " << C.y << "]  output: [" << result.x << ", " << result.y << "]\n";
	return result;
}

bool DesktopDrawer::isLightPen(int R, int G, int B){
	return ((R>220) && (R>G+B));
}

DesktopDrawer::DesktopDrawer(BlackBoard * parent){
   blackBoard = parent;
	desktopWidth=800;
	desktopHeight=600;
	desktop=cvCreateImage(cvSize(desktopWidth,desktopHeight),IPL_DEPTH_8U,3);
	
	int W=desktop->widthStep;
	int H=desktop->height;
	int x,y;
	for(y=0; y<H; y++)
	for(x=0; x<W; x+=3) 
	{
		desktop->imageData[y*W+x+2]=32;
   		desktop->imageData[y*W+x+1]=32;
		desktop->imageData[y*W+x]=32;
	}

   //initialization array for finding maximal square of lightPen pixels
   int captureWidth=cvGetCaptureProperty(blackBoard->webcam,CV_CAP_PROP_FRAME_WIDTH);
	int captureHeight=cvGetCaptureProperty(blackBoard->webcam,CV_CAP_PROP_FRAME_HEIGHT);
	cerr << "capture resolution is " << captureWidth << "x" << captureHeight << endl;
	for(int i=0; i<captureHeight/2; i++)
		maxSquare[i] = (int*)malloc(sizeof(int)*(captureWidth+4)/2);		
}

DesktopDrawer::~DesktopDrawer(){
	int captureHeight=cvGetCaptureProperty(blackBoard->webcam,CV_CAP_PROP_FRAME_HEIGHT);
	for(int i=0; i<captureHeight/2; i++)
		free(maxSquare[i]);
		
   cvReleaseImage(&desktop);
}
