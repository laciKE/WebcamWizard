#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "blackboard.hpp"

using namespace std;

//calibration of desktop
//result will be stored in calibrationData (vertices of desktop's rectangle in clockwise order from top-left vertex)
//return 1 iff success, otherwise 0
//(Tono)
int BlackBoard::calibrate(){

	//show red screen for autocalibration	
	int W=desktop->widthStep;
	int H=desktop->height;
	int x,y;
	for(y=0; y<H; y++)
	for(x=0; x<W; x+=3) 
	{
		desktop->imageData[y*W+x+2]=255;
   		desktop->imageData[y*W+x+1]=0;
		desktop->imageData[y*W+x]=0;
	}

	cvShowImage("BlackBoard",desktop);

	//capture frame from webcam
	IplImage * frame=cvQueryFrame(webcam);
	if(!frame)
		return 0;

	//TODO calibration



	calibrationData.vertex[0]=cvPoint(0,0);
	calibrationData.vertex[1]=cvPoint(639,0);
	calibrationData.vertex[2]=cvPoint(639,479);
	calibrationData.vertex[3]=cvPoint(0,479);

	return 1;
}

//update pracovnej plochy tabule podla zachyteneho snimku z kamery, 
//vysledok sa ulozi do tabula.desktop
//vrati 1 v pripade uspechu, inak 0
//(Laco)
int BlackBoard::update(){
	IplImage * frame=cvQueryFrame(webcam);
	if(!frame) return 0;

	int W=frame->widthStep;
	int H=frame->height;
	int x,y;
	for(y=0; y<H; y++)
	for(x=0; x<W; x+=3) 
	{
		int R=(unsigned char)frame->imageData[y*W+x+2];
   		int G=(unsigned char)frame->imageData[y*W+x+1];
		int B=(unsigned char)frame->imageData[y*W+x];
		if(isLightPen(R,G,B)){
			CvPoint pixel=getDesktopCoords(x/3,y);
			desktop->imageData[pixel.y*desktop->widthStep+3*pixel.x+2]=255;
			desktop->imageData[pixel.y*desktop->widthStep+3*pixel.x+1]=0;
			desktop->imageData[pixel.y*desktop->widthStep+3*pixel.x+0]=0;
		}
	}

	IplImage * resizedFrame=cvCreateImage(cvSize(320, 240), frame->depth, frame->nChannels );
	cvResize(frame,resizedFrame);
	cvShowImage("Webcam",resizedFrame);
	cvReleaseImage(&resizedFrame);
	cvShowImage("BlackBoard",desktop);

	return 1;	
}

CvPoint BlackBoard::getDesktopCoords(int x, int y){
	//binary search coordinates of webcam point (x,y) in rectangle in calibrationData
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

bool BlackBoard::isLightPen(int R, int G, int B){
	return ((R>200) && (R>G+B));
}

BlackBoard::~BlackBoard(){
	cvReleaseCapture(&webcam);
	cvReleaseImage(&desktop);
	cvDestroyWindow("BlackBoard");
}

BlackBoard::BlackBoard(){
	cvNamedWindow("BlackBoard", CV_WINDOW_AUTOSIZE );
	cvMoveWindow("BlackBoard",328,0);

	cvNamedWindow("Webcam",CV_WINDOW_AUTOSIZE);
	cvMoveWindow("Webcam",0,0);
	//cvSetWindowProperty("BlackBoard",CV_WND_PROP_FULLSCREEN,0);

	webcam=cvCaptureFromCAM(CV_CAP_ANY);
	//webcam=cvCreateCameraCapture(CV_CAP_ANY); 
	if(!webcam){
		cerr << "Create webcam capture failed\n";
		exit(1);
	}

	desktopWidth=800;
	desktopHeight=600;
	desktop=cvCreateImage(cvSize(desktopWidth,desktopHeight),IPL_DEPTH_8U,3);
}

void BlackBoard::initDesktop(){
	calibrate();
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


}
