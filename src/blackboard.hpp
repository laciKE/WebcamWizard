#ifndef BLACKBOARD_HPP
#define BLACKBOARD_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

//zakladne zlozky tabul
class BlackBoard {
	private:
		CvCapture * webcam;
		IplImage *desktop;
		//suradnice 4 rohov tabule, ako ich vidi kamera
		struct{
			CvPoint vertex[4];
		} calibrationData;
		int desktopWidth,desktopHeight;
		CvPoint getDesktopCoords(int x, int y);
		bool isLightPen(int R, int G, int B);
		int calibrate();
	public:
		int update();
		void initDesktop();
		BlackBoard();
		~BlackBoard();
};

#endif
