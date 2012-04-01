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

        public:
		View(QWidget *parent = 0);
                void refresh();
 
private slots:
                void on_pushButton_clicked();
};
 
 
#endif
