#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QtGui> 
#include "view.hpp"
#include "model.hpp"

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
        model = new Model(this);
        model->init();

}

void View::refresh(){ // casovac vola refresh nejaky interval
IplImage *img = model->getBlackBoardImage(); // IPLImage
QImage *qimg = IplImage2QImage(img);
this->label->setPixmap(QPixmap::fromImage(*qimg));
//Label.setPixmap(QPixmap::fromImage(myImage));
}

void View::on_pushButton_clicked()
{
    model->calibrate();
}
