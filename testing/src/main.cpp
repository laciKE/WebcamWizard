#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "tabula.hpp"

using namespace std;

int main(int argc, char** argv)
{
	//vytvorenie a kalibracia novej tabule
	Tabula * tabula = new Tabula();
	tabula->calibrate();
	//obnovujem obraz na tabuli, kym nie je stlaceny ESC
	while(tabula->update()) 
	{
		char c=cvWaitKey(33);
		if(c==27) break;
	}
	delete tabula;

	return 0;
}
