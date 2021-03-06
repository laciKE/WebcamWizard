#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/ml.h>
#include <QWaitCondition>
#include <QMutex>
#include "model.hpp"
#include "calibrator.hpp"

using namespace std;

/*custom sleep, because cvWaitKey doesn't work in Qt application */
void sleep(int ms)
{
	QWaitCondition sleepCond;
	QMutex mutex;
	sleepCond.wait(&mutex, ms);
}

int thresh = 10;
IplImage *img = 0;
IplImage *img0 = 0;
CvMemStorage *storage = 0;
CvPoint calib[4];
double angle(CvPoint *pt1, CvPoint *pt2, CvPoint *pt0)
{
	double dx1 = pt1->x - pt0->x;
	double dy1 = pt1->y - pt0->y;
	double dx2 = pt2->x - pt0->x;
	double dy2 = pt2->y - pt0->y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2
	                                      + dy2 * dy2) + 1e-10);
}

// function from examples opencv (squares.c)

void Calibrator::DetectAndDrawQuads(IplImage *img)
{
	CvSeq *contours;
	CvSeq *result;
	CvMemStorage *storage = cvCreateMemStorage(0);
	// IplImage* ret = cvCreateImage(cvGetSize(img), 8, 1);
	cvFindContours(img, storage, &contours, sizeof(CvContour), CV_RETR_LIST,
	               CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	while (contours)
	{
		double s, t;
		// approximate contour with accuracy proportional
		// to the contour perimeter
		result = cvApproxPoly(contours, sizeof(CvContour), storage,
		                      CV_POLY_APPROX_DP, cvContourPerimeter(contours) * 0.02, 0);
		// square contours should have 4 vertices after approximation
		// relatively large area (to filter out noisy contours)
		// and be convex.
		// Note: absolute value of an area is used because
		// area may be positive or negative - in accordance with the
		// contour orientation
		if (result->total == 4 &&
		        //cvContourArea(result,CV_WHOLE_SEQ,0) > 1000 && //comment by laciKE because compilation error
		        cvContourArea(result, CV_WHOLE_SEQ) > 1000
		        && cvCheckContourConvexity(result))
		{
			s = 0;

			for (int i = 0; i < 5; i++)
			{
				// find minimum angle between joint
				// edges (maximum of cosine)
				if (i >= 2)
				{
					t = fabs(angle((CvPoint *) cvGetSeqElem(result, i),
					               (CvPoint *) cvGetSeqElem(result, i - 2),
					               (CvPoint *) cvGetSeqElem(result, i - 1)));
					s = s > t ? s : t;
				}
			}

			// if cosines of all angles are small
			// (all angles are ~90 degree) then write quandrange
			// vertices to resultant sequence
			if (s < 0.3)
			{
				CvPoint *pt[4];
				for (int i = 0; i < 4; i++)
				{

					// save position rectangle to calib
					pt[i] = (CvPoint *) cvGetSeqElem(result, i);
					CvPoint *Tmp = (CvPoint *) cvGetSeqElem(result, i);
					calib[i] = *Tmp;
				}

				// draw rectangle
				//cvLine(ret, *pt[0], *pt[1], cvScalar(255));
				//cvLine(ret, *pt[1], *pt[2], cvScalar(255));
				//cvLine(ret, *pt[2], *pt[3], cvScalar(255));
				//cvLine(ret, *pt[3], *pt[0], cvScalar(255));
			}

		}

		// take the next contour
		contours = contours->h_next;
	}

	cvReleaseMemStorage(&storage);
}

//calibration of desktop
//result will be stored in calibrationData (vertices of desktop's rectangle in clockwise order from top-left vertex)
//(Tono)
int Calibrator::calibrate()
{
	debug("Calibration start"); 
	IplImage *desktop = cvCreateImage(cvSize(model->blackBoardWidth,
	                                  model->blackBoardHeight), IPL_DEPTH_8U, 3);

	//show white screen for autocalibration
	int W = desktop->widthStep;
	int H = desktop->height;
	int x, y;

	for (y = 0; y < H; y++)
		for (x = 0; x < W; x++)
		{
			desktop->imageData[y * W + x] = 255;
		}

	model->setBlackBoardImage(desktop);

	debug("white image for detection blackboard");
	sleep(2000);

	//capture frame from webcam
	IplImage *frame = cvQueryFrame(model->webcam);

	// some times frames for initializing webcam(problem with modern webcam)
	for (int i = 0; i < 30; i++)
	{
		frame = cvQueryFrame(model->webcam);
		if (!frame)
		{
			return 0;
		}
	}

	// frame in gray scale
	IplImage *frame_gray = cvCreateImage(cvSize(frame->width, frame->height),
	                                     frame->depth, 1);

	// convert frame to gray(BRG format)
	cvCvtColor(frame, frame_gray, CV_BGR2GRAY);

	// frame in BW scale
	IplImage *frame_bw = cvCreateImage(cvGetSize(frame_gray), IPL_DEPTH_8U, 1);

	// convert frame to BW
	cvThreshold(frame_gray, frame_bw, 192, 255, CV_THRESH_BINARY
	            | CV_THRESH_OTSU);

	//show frame_bw


	// function DetectAntDrawQuads need storage
	// init storage
	storage = cvCreateMemStorage(0);

	// show frame with rectangle

	DetectAndDrawQuads(frame_bw);
	cvReleaseImage(&frame_bw);

	// fixed problem with sequence of calib
	int minxy = 500600;
	int mini = 0;

	for (int i = 0; i < 4; i++)
	{
		if (calib[i].x + calib[i].y < minxy)
		{
			minxy = calib[i].x + calib[i].y;
			mini = i;
		}
	}

	int current;
	if (calib[(mini + 1) % 4].x > calib[mini].x)
	{
		current = 1;
	}
	else
	{
		current = -1;
	}

	for (int i = 0; i < 4; i++)
	{
		/*
		char str[256];
		sprintf(str, "%d:%d %d", i, calibrationData.vertex[i].x,
		        calibrationData.vertex[i].y);
		debug(str);
		*/
		calibrationData.vertex[i] = calib[(8 + mini + current * i) % 4];

	}

	cvReleaseImage(&desktop);
	cvReleaseImage(&frame_gray);
	cvReleaseImage(&frame_bw);

	debug("calibration end");

	bool test = false;
	for (int i = 0; i < 4; i++)
	{
		if ((calibrationData.vertex[i].x != 0) || (calibrationData.vertex[i].y
		        != 0))
		{
			test = true;
		}
	}

	if (test == false)
	{
		// manual calibration for testing purpose
		calibrationData.vertex[0] = cvPoint(0, 0);
		calibrationData.vertex[1] = cvPoint(640, 0);
		calibrationData.vertex[2] = cvPoint(640, 480);
		calibrationData.vertex[3] = cvPoint(0, 480);
		debug("Calibration failed. Calibration is on manual mod.");
		QMessageBox msgBox;
		msgBox.setWindowTitle("Calibration");
		msgBox.setText("Calibration failed. Calibration is on manual mod.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
		return 0;
	}
	else
	{
		debug("Calibration successfull.");
		IplImage *quads = cvCreateImage(cvSize(model->blackBoardWidth,
		                                       model->blackBoardHeight), 8, 3);

		cvLine(frame, calibrationData.vertex[0], calibrationData.vertex[1],
		       CV_RGB(255, 255, 255));
		cvLine(frame, calibrationData.vertex[1], calibrationData.vertex[2],
		       CV_RGB(255, 255, 255));
		cvLine(frame, calibrationData.vertex[2], calibrationData.vertex[3],
		       CV_RGB(255, 255, 255));
		cvLine(frame, calibrationData.vertex[3], calibrationData.vertex[0],
		       CV_RGB(255, 255, 255));
		cvResize(frame, quads);

		model->setBlackBoardImage(quads);
		cvReleaseImage(&quads);

		QMessageBox msgBox;
		msgBox.setWindowTitle("Calibration");
		msgBox.setText("Calibration successfull.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
		return 1;
	}
}

Calibrator::Calibrator(Model *parent)
{
	model = parent;
}

Calibrator::~Calibrator()
{

}

void Calibrator::debug(const char *str)
{
	model->debug(str);

}
