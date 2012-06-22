#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <QtGui>
#include "model.hpp"
#include "debugview.hpp"

// if we include <QtGui> there is no need to include every class used: <QString>, <QFileDialog>,...

//copy from website
QImage* DebugView::IplImage2QImage(IplImage *iplImg) {
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

DebugView::DebugView( Model *model, QWidget *parent)
{
        setupUi(this); // this sets up GUI
        this->model = model;
}

void DebugView::refresh(){ // timer call refresh
IplImage *img2 = model->getWebcamImage(); // IPLImage
QImage *qimg2 = IplImage2QImage(img2);
//this->camera->setPixmap(QPixmap::fromImage(*qimg2));
delete qimg2;
cvReleaseImage(&img2);
//Label.setPixmap(QPixmap::fromImage(myImage));
}

void DebugView::debug(QString str) {
    debugOutput->append(str); // write message to debugOutput
}
