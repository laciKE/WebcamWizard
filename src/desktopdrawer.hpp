#ifndef DESKTOPDRAWER_HPP
#define DESKTOPDRAWER_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "blackboard.hpp"
#include "pathfinder.hpp"

class DesktopDrawer {
private:
	class BlackBoard* blackBoard;
	class PathFinder* pathFinder;
	IplImage* desktop;
public:
	static const int desktopWidth;
        static const int desktopHeight;
	int update();
	void clear();
	void setPathFinder(PathFinder* pF);
	DesktopDrawer(BlackBoard* parent);
	~DesktopDrawer();
};

#endif
