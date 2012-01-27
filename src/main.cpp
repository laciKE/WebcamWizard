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
	PathFinder* maxSquare = new PathFinderMaxSquare(blackBoard);
	PathFinder* fitLine = new PathFinderFitLine(blackBoard);

	blackBoard->desktopDrawer->setPathFinder(allRed);

	cout << "Ovladanie:\n";
	cout << "ESC     quit\n";
	cout << "c       clear desktop\n";
	cout << "1       PathFinderAllRed\n";
	cout << "2       PathFinderMaxSquare\n";
	cout << "3       PathFinderFitLine\n";

	//update desktop view
	bool run = 1;
	while (blackBoard->update() && run) {
		char c = cvWaitKey(33);
		switch(c){
			case 27:
				run = 0;
				break; //ESC
			case 'c':
				blackBoard->desktopDrawer->clear();
				break;
			case '1':
				blackBoard->desktopDrawer->setPathFinder(allRed);
				break;
			case '2':
				blackBoard->desktopDrawer->setPathFinder(maxSquare);
				break;
			case '3':
				blackBoard->desktopDrawer->setPathFinder(fitLine);
				break;
		}
	}
	delete blackBoard;
	delete allRed;
	delete maxSquare;
	delete fitLine;

	return 0;
}
