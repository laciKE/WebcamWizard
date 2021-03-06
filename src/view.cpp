#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <QtGui>
#include <QFileDialog>
#include "view.hpp"
#include "model.hpp"
#include "pathfinder.hpp"

// if we include <QtGui> there is no need to include every class used: <QString>, <QFileDialog>,...


//copy from website
QImage *View::IplImage2QImage(IplImage *iplImg)
{
	int h = iplImg->height;
	int w = iplImg->width;
	int channels = iplImg->nChannels;
	QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
	char *data = iplImg->imageData;

	for (int y = 0; y < h; y++, data += iplImg->widthStep)
	{
		for (int x = 0; x < w; x++)
		{
			char r, g, b, a;
			r = g = b = a = 0;
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

View::View(Model *model, QWidget *parent)
{
	Q_UNUSED(parent);
	setupUi(this); // this sets up GUI
	debugWidget->hide();
	debugWidget->setFeatures(QDockWidget::DockWidgetMovable);
	//debugWidget->setFeatures(QDockWidget::);
	//debugWidget->mouseGrabber()
	desktop->setStyleSheet("QLabel { background-color : black; color : blue; }");
	colorButton->setStyleSheet("background-color : yellow");

	timer = new QTimer(this);
	this->model = model;

	connect(timer, SIGNAL(timeout()), this, SLOT(refreshSlot()));


}

View::~View()
{
	timer->stop();
	delete timer;
}

void View::refresh()  // timer call refresh
{
	IplImage *img = model->getBlackBoardImage(); // IplImage
	QImage *qimg = IplImage2QImage(img);
	this->desktop->setPixmap(QPixmap::fromImage(*qimg));
	delete qimg;
	cvReleaseImage(&img);

	img = model->getWebcamImage(); // IplImage
	qimg = IplImage2QImage(img);
	this->camera->setPixmap(QPixmap::fromImage(*qimg));
	delete qimg;
	cvReleaseImage(&img);
	//QApplication::flush();
	QCoreApplication::processEvents();
}

void View::refreshSlot()
{
	model->update();
	refresh();
}

void View::debug(QString str)
{
	qDebug() << str;
	debugOutput->append(str);
	QCoreApplication::processEvents();
}

int View::getDesktopWidth()
{
	return View::desktop->width();
}

int View::getDesktopHeight()
{
	return View::desktop->height();
}


void View::on_debugButton_clicked()
{

	if (debugButton->text() == "Debug")
	{
		debugWidget->show();
		debugButton->setText("Hide debug");
	}
	else
	{
		debugWidget->hide();
		debugButton->setText("Debug");
	}
}

void View::on_calibrateButton_clicked()
{
	model->calibrate();

	timer->start(33);
}

void View::on_clearButton_clicked()
{
	model->clear();
}

void View::on_aboutButton_clicked()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("About");
	msgBox.setText("Application was created by Ladislav Baco and Anton Koval, 2011-2012");
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setInformativeText("Webcam Wizard is school project at Comenius University, Faculty of Mathematics, Physics and Informatics.");
	msgBox.setDefaultButton(QMessageBox::Ok);
	msgBox.exec();
}

void View::on_loadButton_clicked()
{
	QString openFilename = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::currentPath(), tr("Image files (*.jpg)"), 0, QFileDialog::DontUseNativeDialog);
	if (!openFilename.isNull())
	{
		IplImage *img = cvLoadImage(openFilename.toAscii());
		model->setBlackBoardImage(img);

	}
	//debug(filename);
}

void View::on_saveButton_clicked()
{
	QString saveFilename = QFileDialog::getSaveFileName(this, tr("Save Image"), QDir::currentPath(), tr("Image files (*.jpg)"), 0, QFileDialog::DontUseNativeDialog);
	if (!saveFilename.isNull())
	{
		if (!saveFilename.contains(".jpg"))
		{
			saveFilename.append(".jpg");
		}
		IplImage *img = model->getBlackBoardImage();
		cvSaveImage(saveFilename.toAscii(), img);
		QMessageBox msgBox;
		msgBox.setWindowTitle("Save image");
		msgBox.setText("Image saved.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
	}
}

void View::on_colorButton_clicked()
{
	QColor color = QColorDialog::getColor(Qt::white, this);
	QString style = "background-color: rgb(%1, %2, %3);";
	colorButton->setStyleSheet(style.arg(color.red()).arg(color.green()).arg(color.blue()));
	model->setColor(CV_RGB(color.red(), color.green(), color.blue()));
}

void View::on_spinBox_valueChanged(int thickness)
{
	model->setThickness(thickness);
}

void View::on_strategyBox_currentIndexChanged(int index)
{
	model->setTool(index);
}
