#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "pathfinder.hpp"
#include "model.hpp"

using namespace std;

inline double cross(CvPoint A, CvPoint B)
{
	return A.x * B.y - A.y * B.x;
}

bool PathFinder::isInteriorPixel(int x, int y)
{
	//is webcam point (x,y) in rectangle in blackBoard->calibrator->calibrationData?
	CalibrationData calibrationData = model->calibrator->calibrationData;
	//is P(x,y) in triangle ABC or triangle ACD? use barycentric coordinates for verification
	const double epsilon = 0.0005;
	CvPoint A = cvPoint(0, 0);
	CvPoint B = cvPoint(calibrationData.vertex[1].x - calibrationData.vertex[0].x, calibrationData.vertex[1].y - calibrationData.vertex[0].y);
	CvPoint C = cvPoint(calibrationData.vertex[2].x - calibrationData.vertex[0].x, calibrationData.vertex[2].y - calibrationData.vertex[0].y);
	CvPoint D = cvPoint(calibrationData.vertex[3].x - calibrationData.vertex[0].x, calibrationData.vertex[3].y - calibrationData.vertex[0].y);
	CvPoint P = cvPoint(x - calibrationData.vertex[0].x, y - calibrationData.vertex[0].y);

	//P in triangle ABC?
	double S = cross(B, C);
	double a = cross(B, P) / S;
	double b = cross(cvPoint(B.x - P.x, B.y - P.y), cvPoint(C.x - P.x, C.y - P.y)) / S;
	double c = cross(P, C) / S;
	if (abs(a + b + c - 1) < epsilon)
	{
		return true;
	}

	//P in triangle ACD?
	S = cross(C, D);
	a = cross(C, P) / S;
	b = cross(cvPoint(C.x - P.x, C.y - P.y), cvPoint(D.x - P.x, D.y - P.y)) / S;
	c = cross(P, D) / S;
	if (abs(a + b + c - 1) < epsilon)
	{
		return true;
	}

	return false;
}

CvPoint PathFinder::getDesktopCoords(int x, int y)
{
	//binary search coordinates of webcam point (x,y) in rectangle in blackBoard->calibrator->calibrationData
	CalibrationData calibrationData = model->calibrator->calibrationData;
	const double epsilon = 0.005;
	double l, r, p;
	CvPoint A, B, C = cvPoint(x, y);
	CvPoint result;
	//x coordinate
	l = 0;
	r = 1;
	while (r - l > epsilon)
	{
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
		{
			r = p;
		}
		else
		{
			l = p;
		}
	}
	result.x = model->blackBoardWidth * l;
	//float l0 = l;
	//cerr << l << " ";
	//y coordinate
	l = 0;
	r = 1;
	while (r - l > epsilon)
	{
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
		{
			r = p;
		}
		else
		{
			l = p;
		}
	}
	result.y = model->blackBoardHeight * (1 - l);
	//cerr << 1-l << endl;
	//cerr << "input: [" << C.x << ", " << C.y << "]  output: [" << result.x << ", " << result.y << "]\n";
	/*if((l<2*epsilon) || (l>1-2*epsilon) || (l0<2*epsilon) || (l0>1-2*epsilon)){
	    result.x = result.y = 0;
	}*/
	return result;
}

inline bool PathFinder::isLightPen(int R, int G, int B)
{
	//return ((R > 220) && (R > (G + B) * 2 / 3));
	return ((B > 245) && (B > (G + R) * 3 / 4));
}


PathFinder::PathFinder(Model *m)
{
	model = m;
}

PathFinder::~PathFinder()
{

}



//Find and draw all red pixels from frame

void PathFinderAllRed::Init()
{
	model->debug("PathFinderAllRed");
}

void PathFinderAllRed::drawPath(IplImage *frame, IplImage *desktop, const CvScalar &color, int thickness)
{
	int W = frame->widthStep;
	int H = frame->height;
	int x, y;
	for (y = 1; y < H; y++)
		for (x = 3; x < W; x += 3)
		{
			int R = (unsigned char) frame->imageData[y * W + x + 2];
			int G = (unsigned char) frame->imageData[y * W + x + 1];
			int B = (unsigned char) frame->imageData[y * W + x];
			if (isLightPen(R, G, B) && isInteriorPixel(x / 3, y))
			{
				CvPoint pixel = getDesktopCoords(x / 3, y);
				cvLine(desktop, pixel, pixel, color, thickness);
			}
		}
}

PathFinderAllRed::PathFinderAllRed(Model *m): PathFinder(m)
{
}

PathFinderAllRed::~PathFinderAllRed()
{
}



//Find max lightpen square in frame and draw line with endpoints in center of two squares

void PathFinderMaxSquare::Init()
{
	lastPoint.x = -1;
	lastPoint.y = -1;
	model->debug("PathFinderMaxSquare");
}

void PathFinderMaxSquare::drawPath(IplImage *frame, IplImage *desktop, const CvScalar &color, int thickness)
{
	int W = frame->widthStep;
	int H = frame->height;
	int x, y;
	for (y = 0; y < H; y++)
	{
		memset(maxSquare[y], 0, W / 3);
	}

	int maxX, maxY, maxVal;
	maxX = maxY = maxVal = 0;
	for (y = 1; y < H; y++)
		for (x = 3; x < W; x += 3)
		{
			int R = (unsigned char) frame->imageData[y * W + x + 2];
			int G = (unsigned char) frame->imageData[y * W + x + 1];
			int B = (unsigned char) frame->imageData[y * W + x];
			if (isLightPen(R, G, B) && isInteriorPixel(x / 3, y))
			{
				maxSquare[y][x / 3] = min(maxSquare[y - 1][x / 3], min(maxSquare[y][x / 3 - 1], maxSquare[y - 1][x / 3 - 1])) + 1;
				if (maxSquare[y][x / 3] > maxVal)
				{
					maxVal = maxSquare[y][x / 3];
					maxX = x / 3;
					maxY = y;
				}
			}
			else
			{
				maxSquare[y][x / 3] = 0;
			}
		}
	if (maxVal > 1)
	{
		CvPoint pixel = getDesktopCoords(maxX - maxVal / 2, maxY - maxVal / 2);
		if (lastPoint.x > 0)
		{
			cvLine(desktop, lastPoint, pixel, color, thickness);
		}
		else
		{
			cvLine(desktop, pixel, pixel, color, thickness);
		}

		lastPoint = pixel;
	}
	else
	{
		lastPoint = cvPoint(-1, -1);
	}
}

PathFinderMaxSquare::PathFinderMaxSquare(Model *m): PathFinder(m)
{
	//initialization array for finding maximal square of lightPen pixels
	int captureWidth = cvGetCaptureProperty(model->webcam,
	                                        CV_CAP_PROP_FRAME_WIDTH);
	int captureHeight = cvGetCaptureProperty(model->webcam,
	                    CV_CAP_PROP_FRAME_HEIGHT);
	if (!captureWidth)
	{
		captureWidth = 640;
		captureHeight = 480;
	}
	maxSquare = (int **) malloc(sizeof(int *) * (captureHeight + 4));
	for (int i = 0; i < captureHeight; i++)
	{
		maxSquare[i] = (int *) malloc(sizeof(int) * (captureWidth + 4));
	}
}

PathFinderMaxSquare::~PathFinderMaxSquare()
{
	int captureHeight = cvGetCaptureProperty(model->webcam,
	                    CV_CAP_PROP_FRAME_HEIGHT);
	for (int i = 0; i < captureHeight; i++)
	{
		free(maxSquare[i]);
	}
	free(maxSquare);
}



//Fit lightpen pixels in frame with linea and draw it, connected to previous line, if possible

void PathFinderFitLine::Init()
{
	lastPoint.x = -1;
	lastPoint.y = -1;

	cvClearSeq(point_seq);

	model->debug("PathFinderFitLine");
}

void PathFinderFitLine::drawPath(IplImage *frame, IplImage *desktop, const CvScalar &color, int thickness)
{
	int W = frame->widthStep;
	int x, y, numOfPoints = 0;
	cvClearSeq(point_seq);
	float line[4];
	vector <CvPoint2D32f> points;

	for (y = min(model->calibrator->calibrationData.vertex[0].y, model->calibrator->calibrationData.vertex[1].y); y < max(model->calibrator->calibrationData.vertex[2].y, model->calibrator->calibrationData.vertex[3].y); y++)
		for (x = 3 * min(model->calibrator->calibrationData.vertex[0].x, model->calibrator->calibrationData.vertex[1].x); x < 3 * max(model->calibrator->calibrationData.vertex[2].x, model->calibrator->calibrationData.vertex[3].x); x += 3)
		{
			int R = (unsigned char) frame->imageData[y * W + x + 2];
			int G = (unsigned char) frame->imageData[y * W + x + 1];
			int B = (unsigned char) frame->imageData[y * W + x];
			if (isLightPen(R, G, B) && isInteriorPixel(x / 3, y))
			{
				points.push_back(cvPoint2D32f(x / 3, y));
				cvSeqPush(point_seq, &points[numOfPoints++]);
			}
		}

	if (numOfPoints > 3)
	{
		cvFitLine(point_seq, CV_DIST_L2, 0, 0.1, 0.1, line);

		CvPoint pixel = getDesktopCoords(line[2], line[3]);
		if (pixel.x > 0)
		{

			if (lastPoint.x > 0)
			{
				cvLine(desktop, lastPoint, pixel, color, thickness);
			}
			else
			{
				cvLine(desktop, pixel, pixel, color, thickness);
			}

			lastPoint = pixel;
		}
	}
	else
	{
		lastPoint = cvPoint(-1, -1);
	}
}

PathFinderFitLine::PathFinderFitLine(Model *m): PathFinder(m)
{
	storage = cvCreateMemStorage(0);
	point_seq = cvCreateSeq(CV_32FC2, sizeof(CvSeq), sizeof(CvPoint2D32f), storage);
}

PathFinderFitLine::~PathFinderFitLine()
{
	cvClearSeq(point_seq);
	cvReleaseMemStorage(&storage);
}


//Compute average from all lightpen pixels and draw a line connected to prevoius pixel, if possible

void PathFinderAverage::Init()
{
	lastPoint.x = -1;
	lastPoint.y = -1;

	model->debug("PathFinderAverage");
}

void PathFinderAverage::drawPath(IplImage *frame, IplImage *desktop, const CvScalar &color, int thickness)
{
	int W = frame->widthStep;
	int x, y, numOfPoints = 0;
	CvPoint average = cvPoint(0, 0);

	for (y = min(model->calibrator->calibrationData.vertex[0].y, model->calibrator->calibrationData.vertex[1].y); y < max(model->calibrator->calibrationData.vertex[2].y, model->calibrator->calibrationData.vertex[3].y); y++)
		for (x = 3 * min(model->calibrator->calibrationData.vertex[0].x, model->calibrator->calibrationData.vertex[1].x); x < 3 * max(model->calibrator->calibrationData.vertex[2].x, model->calibrator->calibrationData.vertex[3].x); x += 3)
		{
			int R = (unsigned char) frame->imageData[y * W + x + 2];
			int G = (unsigned char) frame->imageData[y * W + x + 1];
			int B = (unsigned char) frame->imageData[y * W + x];
			if (isLightPen(R, G, B) && isInteriorPixel(x / 3, y))
			{
				average.x += x / 3;
				average.y += y;
				numOfPoints++;
			}
		}

	if (numOfPoints > 0)
	{
		average.x /= numOfPoints;
		average.y /= numOfPoints;
		CvPoint pixel = getDesktopCoords(average.x, average.y);
		if (pixel.x > 0)
		{

			if (lastPoint.x > 0)
			{
				cvLine(desktop, lastPoint, pixel, color, thickness);
			}
			else
			{
				cvLine(desktop, pixel, pixel, color, thickness);
			}

			lastPoint = pixel;
		}
	}
	else
	{
		lastPoint = cvPoint(-1, -1);
	}
}

PathFinderAverage::PathFinderAverage(Model *m): PathFinder(m)
{
}

PathFinderAverage::~PathFinderAverage()
{
}


//draw line segments from first seen lightpen mark to last seen lightpen mark
//lightpen marks are computed using average strategy

void PathFinderLineSegment::Init()
{
	firstPoint = lastPoint = cvPoint(-1, -1);

	model->debug("PathFinderLineSegment");
}

void PathFinderLineSegment::drawPath(IplImage *frame, IplImage *desktop, const CvScalar &color, int thickness)
{
	int W = frame->widthStep;
	int x, y, numOfPoints = 0;
	CvPoint average = cvPoint(0, 0);

	for (y = min(model->calibrator->calibrationData.vertex[0].y, model->calibrator->calibrationData.vertex[1].y); y < max(model->calibrator->calibrationData.vertex[2].y, model->calibrator->calibrationData.vertex[3].y); y++)
		for (x = 3 * min(model->calibrator->calibrationData.vertex[0].x, model->calibrator->calibrationData.vertex[1].x); x < 3 * max(model->calibrator->calibrationData.vertex[2].x, model->calibrator->calibrationData.vertex[3].x); x += 3)
		{
			int R = (unsigned char) frame->imageData[y * W + x + 2];
			int G = (unsigned char) frame->imageData[y * W + x + 1];
			int B = (unsigned char) frame->imageData[y * W + x];
			if (isLightPen(R, G, B) && isInteriorPixel(x / 3, y))
			{
				average.x += x / 3;
				average.y += y;
				numOfPoints++;
			}
		}

	if (numOfPoints > 0)
	{
		average.x /= numOfPoints;
		average.y /= numOfPoints;
		CvPoint pixel = getDesktopCoords(average.x, average.y);
		if (pixel.x > 0)
		{
			if (firstPoint.x < 0) {
				firstPoint = pixel;
				cvReleaseImage(&oldDesktop);
				oldDesktop = cvCloneImage(desktop);
			}
			lastPoint = pixel;
			cvCopy(oldDesktop, desktop);
			cvLine(desktop, lastPoint, firstPoint, color, thickness);
		}
	}
	else
	{
		firstPoint = lastPoint = cvPoint(-1, -1);
	}
}

PathFinderLineSegment::PathFinderLineSegment(Model *m): PathFinder(m)
{
	oldDesktop = cvCreateImage(cvSize(32, 32), 8, 3); //something not null
}

PathFinderLineSegment::~PathFinderLineSegment()
{
	cvReleaseImage(&oldDesktop);
}
