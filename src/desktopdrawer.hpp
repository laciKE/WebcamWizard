#ifndef DESKTOPDRAWER_HPP
#define DESKTOPDRAWER_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "blackboard.hpp"

class DesktopDrawer {
private:
	class BlackBoard* blackBoard;
	IplImage* desktop;
	CvPoint lastPoint;
	int* maxSquare[1024]; //array for finding maximal square of lightPen pixels in frame from webcam
	CvPoint getDesktopCoords(int x, int y);
	bool isLightPen(int R, int G, int B);
	void drawPoint(CvPoint A);
	void drawLine(CvPoint A, CvPoint B);
public:

	int desktopWidth, desktopHeight;
	int update();
	void clear();
	DesktopDrawer(BlackBoard* parent);
	~DesktopDrawer();
};

#endif
