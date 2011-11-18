#ifndef BLACKBOARD_HPP
#define BLACKBOARD_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "calibrator.hpp"
#include "desktopdrawer.hpp"

class BlackBoard {
	private:

	public:	
		CvCapture * webcam;
		Calibrator * calibrator;
		DesktopDrawer * desktopDrawer;
		int update();
		char blackBoardWindow[64], webcamWindow[64];
		void Init();
		BlackBoard();
		~BlackBoard();
};

#endif
