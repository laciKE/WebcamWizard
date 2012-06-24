/********************************************************************************
** Form generated from reading UI file 'webcamWizard.ui'
**
** Created: Sun Jun 24 13:40:24 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEBCAMWIZARD_H
#define UI_WEBCAMWIZARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QPushButton *calibrateButton;
    QLabel *desktop;
    QPushButton *saveButton;
    QPushButton *loadButton;
    QPushButton *clearButton;
    QPushButton *colorButton;
    QPushButton *aboutButton;
    QPushButton *debugButton;
    QComboBox *comboBox;
    QDockWidget *debugWidget;
    QWidget *dock;
    QLabel *camera;
    QTextEdit *debugOutput;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(880, 670);
        calibrateButton = new QPushButton(Form);
        calibrateButton->setObjectName(QString::fromUtf8("calibrateButton"));
        calibrateButton->setGeometry(QRect(30, 10, 91, 31));
        desktop = new QLabel(Form);
        desktop->setObjectName(QString::fromUtf8("desktop"));
        desktop->setGeometry(QRect(40, 50, 800, 600));
        desktop->setAutoFillBackground(false);
        desktop->setLineWidth(1);
        saveButton = new QPushButton(Form);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(140, 10, 61, 31));
        loadButton = new QPushButton(Form);
        loadButton->setObjectName(QString::fromUtf8("loadButton"));
        loadButton->setGeometry(QRect(200, 10, 61, 31));
        clearButton = new QPushButton(Form);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setGeometry(QRect(280, 10, 81, 31));
        colorButton = new QPushButton(Form);
        colorButton->setObjectName(QString::fromUtf8("colorButton"));
        colorButton->setGeometry(QRect(360, 10, 81, 31));
        aboutButton = new QPushButton(Form);
        aboutButton->setObjectName(QString::fromUtf8("aboutButton"));
        aboutButton->setGeometry(QRect(760, 10, 81, 31));
        debugButton = new QPushButton(Form);
        debugButton->setObjectName(QString::fromUtf8("debugButton"));
        debugButton->setGeometry(QRect(660, 10, 91, 31));
        comboBox = new QComboBox(Form);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(460, 10, 181, 31));
        debugWidget = new QDockWidget(Form);
        debugWidget->setObjectName(QString::fromUtf8("debugWidget"));
        debugWidget->setEnabled(true);
        debugWidget->setGeometry(QRect(330, 60, 511, 288));
        debugWidget->setMinimumSize(QSize(511, 288));
        debugWidget->setMaximumSize(QSize(511, 288));
        debugWidget->setMouseTracking(true);
        debugWidget->setFloating(true);
        debugWidget->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetVerticalTitleBar);
        debugWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
        dock = new QWidget();
        dock->setObjectName(QString::fromUtf8("dock"));
        camera = new QLabel(dock);
        camera->setObjectName(QString::fromUtf8("camera"));
        camera->setGeometry(QRect(10, 0, 320, 240));
        debugOutput = new QTextEdit(dock);
        debugOutput->setObjectName(QString::fromUtf8("debugOutput"));
        debugOutput->setGeometry(QRect(350, 0, 151, 241));
        debugOutput->setReadOnly(true);
        debugWidget->setWidget(dock);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Webcam Wizard", 0, QApplication::UnicodeUTF8));
        calibrateButton->setText(QApplication::translate("Form", "Calibrate", 0, QApplication::UnicodeUTF8));
        desktop->setText(QString());
        saveButton->setText(QApplication::translate("Form", "Save", 0, QApplication::UnicodeUTF8));
        loadButton->setText(QApplication::translate("Form", "Load", 0, QApplication::UnicodeUTF8));
        clearButton->setText(QApplication::translate("Form", "Clear", 0, QApplication::UnicodeUTF8));
        colorButton->setText(QApplication::translate("Form", "Color", 0, QApplication::UnicodeUTF8));
        aboutButton->setText(QApplication::translate("Form", "About", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        debugButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        debugButton->setText(QApplication::translate("Form", "Debug", 0, QApplication::UnicodeUTF8));
        debugWidget->setWindowTitle(QString());
        camera->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEBCAMWIZARD_H
