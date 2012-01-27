/*
	Strategy design pattern for finding path of red curve in captured frames
*/

#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "blackboard.hpp"

class PathFinder {
protected:
	CvPoint getDesktopCoords(int x, int y);
	BlackBoard* blackBoard;
	bool isLightPen(int R, int G, int B);
	void drawPoint(CvPoint A, IplImage* img);
	void drawLine(CvPoint A, CvPoint B, IplImage* img);
public:
	virtual void Init(){};
	virtual void drawPath(IplImage* frame, IplImage* desktop){};
	PathFinder(BlackBoard* bB);
	~PathFinder();
};


class PathFinderAllRed: public PathFinder {
private:
public:
	void Init();
	void drawPath(IplImage* frame, IplImage* desktop);
	PathFinderAllRed(BlackBoard* bB);
	~PathFinderAllRed();
};


#endif
