#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "tabula.hpp"

using namespace std;

//kalibracia tabule
//vysledok sa ulozi do tabula.calibrationData
//vrati 1 v pripade uspechu, inak 0
//(Tono)
int Tabula::calibrate(){

	//show red screen for autocalibration	
	int W=pracovnaPlocha->widthStep;
	int H=pracovnaPlocha->height;
	int x,y;
	for(y=0; y<H; y++)
	for(x=0; x<W; x+=3) 
	{
		pracovnaPlocha->imageData[y*W+x+2]=255;
   		pracovnaPlocha->imageData[y*W+x+1]=0;
		pracovnaPlocha->imageData[y*W+x]=0;
	}

	cvShowImage("Tabula",pracovnaPlocha);

	//capture frame from webcam
	IplImage * frame=cvQueryFrame(webcam);
	if(!frame)
		return 0;

	//TODO calibration



	for(int i=0; i<4; i++)
		calibrationData.roh[i].x=calibrationData.roh[i].y=0;

	return 1;
}

//update pracovnej plochy tabule podla zachyteneho snimku z kamery, 
//vysledok sa ulozi do tabula.pracovnaPlocha
//vrati 1 v pripade uspechu, inak 0
//(Laco)
int Tabula::update(){
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
	}

	cvShowImage("Tabula",pracovnaPlocha);
	//cvShowImage("Tabula",frame);

	return 1;	
}

Tabula::~Tabula(){
	cvReleaseCapture(&webcam);
	cvDestroyWindow("Tabula");
}

Tabula::Tabula(){
	cvNamedWindow("Tabula", CV_WINDOW_AUTOSIZE );
	webcam=cvCreateCameraCapture(CV_CAP_ANY);
	//TODO check webcam 

	pracovnaPlocha=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
}
