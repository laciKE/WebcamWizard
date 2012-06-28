#ifndef VIEW_HPP
#define VIEW_HPP

#include "ui_webcamWizard.h"
#include "model.hpp"
 
 
class View : public QWidget, private Ui::Form
{
	Q_OBJECT
        private:

        Model *model;
        QImage *IplImage2QImage(IplImage *iplImg);
        QTimer * timer;

        public:

		View(QWidget *parent = 0);
                void refresh();
		void debug(QString str);
		int getDesktopWidth();
		int getDesktopHeight();



private slots:
                void on_debugButton_clicked();
                void on_calibrateButton_clicked();
                void on_clearButton_clicked();
                void on_aboutButton_clicked();
                void on_loadButton_clicked();
                void on_saveButton_clicked();
                void on_colorButton_clicked();

                void on_spinBox_valueChanged(int arg1);

public slots:
                void refreshSlot();
};
 
#endif

