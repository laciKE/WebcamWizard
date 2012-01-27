#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "blackboard.hpp"
#include "pathfinder.hpp"
//#include "pathfinder_all_red.hpp"

using namespace std;

int main(int argc, char** argv) {
	//initialization and calibration of new blackBoard
	BlackBoard* blackBoard = new BlackBoard();
	blackBoard->Init();
	PathFinder* allRed = new PathFinderAllRed(blackBoard);
	blackBoard->desktopDrawer->setPathFinder(allRed);
	//update desktop view
	while (blackBoard->update()) {
		char c = cvWaitKey(33);
		if (c == 27)
			break; //ESC
		if (c == 'c')
			blackBoard->desktopDrawer->clear();
	}
	delete blackBoard;

	return 0;
}
