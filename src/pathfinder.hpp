/*
    Strategy design pattern for finding path of red curve in captured frames
*/

#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "model.hpp"

class PathFinder
{
protected:
	CvPoint getDesktopCoords(int x, int y);
	Model *model;
	bool isLightPen(int R, int G, int B);
	bool isInteriorPixel(int x, int y);
public:
	virtual void Init() {};
	virtual void drawPath(IplImage * /*frame*/, IplImage * /*desktop*/, const CvScalar & /*color*/, int /*thickness*/) {};
	PathFinder(Model *m);
	~PathFinder();
};


class PathFinderAllRed: public PathFinder
{
private:
public:
	void Init();
	void drawPath(IplImage *frame, IplImage *desktop, const CvScalar &color, int thickness);
	PathFinderAllRed(Model *m);
	~PathFinderAllRed();
};

class PathFinderMaxSquare: public PathFinder
{
private:
	int **maxSquare;
	CvPoint lastPoint;
public:
	void Init();
	void drawPath(IplImage *frame, IplImage *desktop, const CvScalar &color, int thickness);
	PathFinderMaxSquare(Model *m);
	~PathFinderMaxSquare();
};

class PathFinderFitLine: public PathFinder
{
private:
	CvMemStorage *storage;
	CvSeq *point_seq;
	CvPoint lastPoint;
public:
	void Init();
	void drawPath(IplImage *frame, IplImage *desktop, const CvScalar &color, int thickness);
	PathFinderFitLine(Model *m);
	~PathFinderFitLine();
};


class PathFinderAverage: public PathFinder
{
private:
	CvPoint lastPoint;
public:
	void Init();
	void drawPath(IplImage *frame, IplImage *desktop, const CvScalar &color, int thickness);
	PathFinderAverage(Model *m);
	~PathFinderAverage();
};

class PathFinderLineSegment: public PathFinder
{
private:
	CvPoint firstPoint, lastPoint;
	IplImage *oldDesktop;
public:
	void Init();
	void drawPath(IplImage *frame, IplImage *desktop, const CvScalar &color, int thickness);
	PathFinderLineSegment(Model *m);
	~PathFinderLineSegment();
};

#endif
