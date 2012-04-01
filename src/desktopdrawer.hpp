#ifndef DESKTOPDRAWER_HPP
#define DESKTOPDRAWER_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "model.hpp"

class DesktopDrawer
{
private:
        //class BlackBoard* blackBoard;
        // class PathFinder* pathFinder;
        IplImage* desktop;
public:
        static const int desktopWidth=800;
        static const int desktopHeight=600;
        int update();
        void clear();
        //void setPathFinder(PathFinder* pF);
        DesktopDrawer(Model* parent);
       // ~DesktopDrawer();

};

#endif // DESKTOPDRAWER_HPP
