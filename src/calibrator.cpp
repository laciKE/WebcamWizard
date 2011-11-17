#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "blackboard.hpp"
#include "calibrator.hpp"

using namespace std;

//calibration of desktop
//result will be stored in calibrationData (vertices of desktop's rectangle in clockwise order from top-left vertex)
//return 1 iff success, otherwise 0
//(Tono)
int Calibrator::calibrate(){
	IplImage * desktop=cvCreateImage(cvSize(blackBoard->desktopDrawer->desktopWidth,blackBoard->desktopDrawer->desktopHeight),IPL_DEPTH_8U,3);
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

	cvShowImage(blackBoard->blackBoardWindow,desktop);

	//capture frame from webcam
	IplImage * frame=cvQueryFrame(blackBoard->webcam);
	if(!frame)
		return 0;

	//TODO calibration



	calibrationData.vertex[0]=cvPoint(0,0);
	calibrationData.vertex[1]=cvPoint(639,0);
	calibrationData.vertex[2]=cvPoint(639,479);
	calibrationData.vertex[3]=cvPoint(0,479);

	return 1;
}

Calibrator::Calibrator(BlackBoard * parent){
	blackBoard = parent;
}

Calibrator::~Calibrator(){

}
