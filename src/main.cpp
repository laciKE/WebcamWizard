#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "blackboard.hpp"

using namespace std;

int main(int argc, char** argv) {
	//vytvorenie a kalibracia novej tabule
	BlackBoard * blackBoard = new BlackBoard();
	blackBoard->Init();
	//obnovujem obraz na tabuli, kym nie je stlaceny ESC
	while (blackBoard->update()) {
		char c = cvWaitKey(33);
		if (c == 27)
			break;
		if (c == 'c')
			blackBoard->desktopDrawer->clear();
	}
	delete blackBoard;

	return 0;
}
