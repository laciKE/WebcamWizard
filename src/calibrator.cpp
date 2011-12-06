#include <cstdio>
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
int Calibrator::calibrate() {
	IplImage* desktop = cvCreateImage(cvSize(
			blackBoard->desktopDrawer->desktopWidth,
			blackBoard->desktopDrawer->desktopHeight), IPL_DEPTH_8U, 3);

	//show white screen for autocalibration
	int W = desktop->widthStep;
	int H = desktop->height;
	int x, y;
	for (y = 0; y < H; y++)
		for (x = 0; x < W; x += 3) {
			desktop->imageData[y * W + x + 2] = 255;
			desktop->imageData[y * W + x + 1] = 255;
			desktop->imageData[y * W + x] = 255;
		}

	cvShowImage(blackBoard->blackBoardWindow, desktop);

    //wait 250ms
	cvWaitKey(250);

	//capture frame from webcam
	IplImage* frame = cvQueryFrame(blackBoard->webcam);
	if (!frame)
		return 0;

    // some times frames for initializing webcam
    for (int i=0; i<30; i++) {
    frame = cvQueryFrame(blackBoard->webcam);
        if (!frame)
          return 0;
    }

    //frame in gray scale
    IplImage* frame1_gray = cvCreateImage(cvGetSize(frame), 8, 1);
    //frame1_gray = cvCloneImage(frame);
    cvCvtColor( frame, frame1_gray, CV_BGR2GRAY );

    //experiment with only white&black colour
    W = frame1_gray->widthStep;
	H = frame1_gray->height;
    for (y = 0; y < H; y++)
		for (x = 0; x < W; x++) {
            if (frame1_gray->imageData[y * W + x] >86) {frame1_gray->imageData[y * W + x]=255;}
            else frame1_gray->imageData[y * W + x] =0;
     }

    //show frame1_gray
	cvShowImage(blackBoard->webcamWindow,frame1_gray);
    cerr << "first frame" << endl;

    //wait 5s
	cvWaitKey(5000);

    //show black screen for autocalibration
    W = desktop->widthStep;
	H = desktop->height;
    for (y = 0; y < H; y++)
		for (x = 0; x < W; x += 3) {
			desktop->imageData[y * W + x + 2] = 0;
			desktop->imageData[y * W + x + 1] = 0;
			desktop->imageData[y * W + x] = 0;
		}

	cvShowImage(blackBoard->blackBoardWindow, desktop);
	cvWaitKey(1000);

    IplImage * frame2 = cvQueryFrame(blackBoard->webcam);
	if (!frame2)
		return 0;

    for (int i=0; i<30; i++) {
    frame2 = cvQueryFrame(blackBoard->webcam);
        if (!frame2)
          return 0;
    }

    IplImage* frame2_gray = cvCreateImage(cvGetSize(frame2), 8, 3);
    //cvCvtColor( frame2, frame2_gray, CV_BGR2GRAY );
    frame2_gray = cvCloneImage(frame2);
    //cvShowImage(blackBoard->webcamWindow,frame2_gray);
    //cvWaitKey(2000);

    W = frame2_gray->widthStep;
	H = frame2_gray->height;
    for (y = 0; y < H; y++){
		for (x = 0; x < W; x++){
		    char c = frame2_gray->imageData[y * W + x ];
            printf("%u ",c);
            }
        printf("\n");
    }

    //experiment with White&Black colour
    for (y = 0; y < H; y++)
		for (x = 0; x < W; x+=3) {

            if ((frame2_gray->imageData[y * W + x ]<60) || (frame2_gray->imageData[y * W + x + 2]<60) || (frame2_gray->imageData[y * W + x + 1]<60)) {
                frame2_gray->imageData[y * W + x + 1] = 0;
                frame2_gray->imageData[y * W + x + 2] = 0;
                frame2_gray->imageData[y * W + x] = 0;
                }
            else {
                frame2_gray->imageData[y * W + x + 1] = 255;
                frame2_gray->imageData[y * W + x + 2] = 255;
                frame2_gray->imageData[y * W + x ] = 255;
            }
     }

    cvShowImage(blackBoard->webcamWindow,frame2_gray);
    cerr << "second frame" << endl;
    //cerr << frame << " " << frame2 << endl;

    cvWaitKey(5000);

    // xor frame1_gray and frame2_gray
    W = frame1_gray->widthStep;
	H = frame1_gray->height;

    for (y = 0; y < H; y++)
		for (x = 0; x < W; x++) {
            frame1_gray->imageData[y * W + x] ^= frame2_gray->imageData[y * W + x];
		}

    cvShowImage(blackBoard->blackBoardWindow,frame1_gray);
    cvWaitKey(10000);

	//TODO calibration
    // manual calibration
	calibrationData.vertex[0] = cvPoint(90, 39);
	calibrationData.vertex[1] = cvPoint(448, 46);
	calibrationData.vertex[2] = cvPoint(480, 309);
	calibrationData.vertex[3] = cvPoint(60, 308);

	cvReleaseImage(&desktop);
	cvReleaseImage(&frame1_gray);
	cvReleaseImage(&frame2_gray);
	//cerr << "calibration end" << endl;
	return 1;
}

Calibrator::Calibrator(BlackBoard* parent) {
	blackBoard = parent;
}

Calibrator::~Calibrator() {

}
