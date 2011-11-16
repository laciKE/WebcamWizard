#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "blackboard.hpp"

using namespace std;

int main(int argc, char** argv)
{
	//vytvorenie a kalibracia novej tabule
	BlackBoard * blackboard = new BlackBoard();
	blackboard->initDesktop();
	//obnovujem obraz na tabuli, kym nie je stlaceny ESC
	while(blackboard->update()) 
	{
		char c=cvWaitKey(33);
		if(c==27) break;
	}
	delete blackboard;

	return 0;
}
