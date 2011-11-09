#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "tabula.hpp"

using namespace std;

//kalibracia tabule
//vysledok sa ulozi do tabula.calibrationData
//vrati 1 v pripade uspechu, inak 0
//(Tono)
int Tabula::calibrate(){

	//TODO kalibracia	

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

	return 1;	
}

Tabula::~Tabula(){
	cvReleaseCapture(&webcam);
	cvDestroyWindow("Tabula");
}

Tabula::Tabula(){
	cvNamedWindow("Tabula", CV_WINDOW_AUTOSIZE );
	webcam=cvCreateCameraCapture(0);
	pracovnaPlocha=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
}
