/*
	Strategy design pattern for finding path of red curve in captured frames
*/

#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "model.hpp"

class PathFinder {
protected:
	CvPoint getDesktopCoords(int x, int y);
	Model *model;
	bool isLightPen(int R, int G, int B);
	void drawPoint(CvPoint A, IplImage* img);
	void drawLine(CvPoint A, CvPoint B, IplImage* img);
public:
        virtual void Init(){};
        virtual void drawPath(IplImage* frame, IplImage* desktop){};
	PathFinder(Model *m);
	~PathFinder();
};


class PathFinderAllRed: public PathFinder {
private:
public:
	void Init();
	void drawPath(IplImage* frame, IplImage* desktop);
	PathFinderAllRed(Model *m);
	~PathFinderAllRed();
};

class PathFinderMaxSquare: public PathFinder {
private:
	int** maxSquare;
	CvPoint lastPoint;	
public:
	void Init();
	void drawPath(IplImage* frame, IplImage* desktop);
	PathFinderMaxSquare(Model *m);
	~PathFinderMaxSquare();
};

class PathFinderFitLine: public PathFinder {
private:
	CvMemStorage* storage;
	CvSeq* point_seq;
	CvPoint lastPoint;	
public:
	void Init();
	void drawPath(IplImage* frame, IplImage* desktop);
	PathFinderFitLine(Model *m);
	~PathFinderFitLine();
};


#endif
