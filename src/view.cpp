#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <QtGui> 
#include "view.hpp"
#include "model.hpp"
#include "pathfinder.hpp"

// if we include <QtGui> there is no need to include every class used: <QString>, <QFileDialog>,...


//copy from website
QImage* View::IplImage2QImage(IplImage *iplImg) {
int h = iplImg->height;
int w = iplImg->width;
int channels = iplImg->nChannels;
QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
char *data = iplImg->imageData;

for (int y = 0; y < h; y++, data += iplImg->widthStep)
{
for (int x = 0; x < w; x++)
{
char r, g, b, a = 0;
if (channels == 1)
{
r = data[x * channels];
g = data[x * channels];
b = data[x * channels];
}
else if (channels == 3 || channels == 4)
{
r = data[x * channels + 2];
g = data[x * channels + 1];
b = data[x * channels];
}

if (channels == 4)
{
a = data[x * channels + 3];
qimg->setPixel(x, y, qRgba(r, g, b, a));
}
else
{
qimg->setPixel(x, y, qRgb(r, g, b));
}
}
}
return qimg;

}

View::View(QWidget *parent)
{
        setupUi(this); // this sets up GUI
        desktop->setStyleSheet("QLabel { background-color : black; color : blue; }");
        timer = new QTimer(this);
        model = new Model(this);
        model->Init();
        connect(timer, SIGNAL(timeout()), this, SLOT(refreshSlot()));


}

void View::refresh(){ // timer call refresh
IplImage *img = model->getBlackBoardImage(); // IPLImage
QImage *qimg = IplImage2QImage(img);
this->desktop->setPixmap(QPixmap::fromImage(*qimg));
delete qimg;
cvReleaseImage(&img);
//Label.setPixmap(QPixmap::fromImage(myImage));

img = model->getWebcamImage(); // IPLImage
qimg = IplImage2QImage(img);
this->camera->setPixmap(QPixmap::fromImage(*qimg));
delete qimg;
cvReleaseImage(&img);
}

void View::refreshSlot() {
    model->update();
    refresh();
   //debugView->refresh();
}


void View::on_pushButton_clicked()
{

}

void View::debug(QString str) {
    debugOutput->append(str);
}

int View::getDesktopWidth(){
    return View::desktop->width();
}

int View::getDesktopHeight(){
    return View::desktop->height();
}


void View::on_debugButton_clicked()
{
    if(debugButton->text().compare("Debug")){
    debugWidget->show();
    debugButton->setText("Hide debug");
    } else {
    debugWidget->hide();
    debugButton->setText("Debug");}
}

void View::on_calibrateButton_clicked()
{
    model->calibrate();
    /*docasne kod kvoli testovaniu */
    //TODO spravit casovac na update
    PathFinder *pH = new PathFinderFitLine(model);
    model->setPathFinder(pH);
    //debugOutput->append(QString("chcem bezat")); //std::cerr << " chcem bezat\n";
    //bool run = 1;
    timer->start(33);


    /*
    //while (model->update() && run) {
            refresh();
        char c = cvWaitKey(33);
        //debugOutput->append(QString("bezim"));  //std::cerr << "bezim\n";
        switch(c){
                case 27:
                        run = 0;
                        break; //ESC
        }
   // }
   */
    //delete pH;
}
