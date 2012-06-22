#ifndef DEBUGVIEW_HPP
#define DEBUGVIEW_HPP

#include "ui_debugForm.h"
#include "model.hpp"

class DebugView : public QWidget, private Ui::DebugForm
{
        Q_OBJECT
        private:
        Model *model;
        QImage *IplImage2QImage(IplImage *iplImg);


        public:
                DebugView(Model *model=0, QWidget *parent=0);
                void refresh();
                void debug(QString str);

};

#endif
