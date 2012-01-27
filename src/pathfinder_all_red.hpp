#ifndef PATHFINDER_ALL_RED_HPP
#define PATHFINDER_ALL_RED_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "pathfinder.hpp"
#include "blackboard.hpp"

class PathFinderAllRed: public PathFinder {
private:
public:
	void Init();
	void drawPath(IplImage* frame, IplImage* desktop);
	PathFinderAllRed(BlackBoard* bB);
	~PathFinderAllRed();
};

#endif
