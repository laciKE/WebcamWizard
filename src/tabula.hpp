#ifndef TABULA_HPP
#define TABULA_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

//zakladne zlozky tabul
class Tabula {
	private:
		CvCapture * webcam;
		IplImage *pracovnaPlocha;
		//suradnice 4 rohov tabule, ako ich vidi kamera
		struct{
			CvPoint roh[4];
		} calibrationData;
	public:
		int calibrate();
		int update();
		Tabula();
		~Tabula();
};

#endif
