#ifndef DESKTOPDRAWER_HPP
#define DESKTOPDRAWER_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "blackboard.hpp"

class DesktopDrawer {
	private:
		IplImage * desktop;
		int * maxSquare[1024]; //array for finding maximal square of lightPen pixels in frame from webcam
		CvPoint getDesktopCoords(int x, int y);
		bool isLightPen(int R, int G, int B);
		BlackBoard * blackBoard;
	public:
		int desktopWidth,desktopHeight;
		int update();
		DesktopDrawer(BlackBoard * parent);
		~DesktopDrawer();
};

#endif
