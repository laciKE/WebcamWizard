/********************************************************************************
** Form generated from reading UI file 'webcamWizard.ui'
**
** Created: Fri Jun 29 10:15:01 2012
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
#include <QtGui/QSpinBox>
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
    QPushButton *aboutButton;
    QPushButton *debugButton;
    QDockWidget *debugWidget;
    QWidget *dock;
    QLabel *camera;
    QTextEdit *debugOutput;
    QSpinBox *spinBox;
    QPushButton *colorButton;
    QComboBox *strategyBox;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(880, 670);
        Form->setMinimumSize(QSize(880, 670));
        Form->setMaximumSize(QSize(880, 670));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(63, 62, 60, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(94, 93, 90, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(78, 77, 75, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(31, 31, 30, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(42, 41, 40, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        QBrush brush6(QColor(0, 0, 0, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
        Form->setPalette(palette);
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
        aboutButton = new QPushButton(Form);
        aboutButton->setObjectName(QString::fromUtf8("aboutButton"));
        aboutButton->setGeometry(QRect(760, 10, 81, 31));
        debugButton = new QPushButton(Form);
        debugButton->setObjectName(QString::fromUtf8("debugButton"));
        debugButton->setGeometry(QRect(660, 10, 91, 31));
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
        spinBox = new QSpinBox(Form);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(380, 12, 41, 27));
        spinBox->setMinimum(1);
        spinBox->setMaximum(10);
        colorButton = new QPushButton(Form);
        colorButton->setObjectName(QString::fromUtf8("colorButton"));
        colorButton->setGeometry(QRect(425, 10, 81, 31));
        strategyBox = new QComboBox(Form);
        strategyBox->setObjectName(QString::fromUtf8("strategyBox"));
        strategyBox->setGeometry(QRect(510, 10, 141, 31));
        strategyBox->setModelColumn(0);

        retranslateUi(Form);

        strategyBox->setCurrentIndex(2);


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
        aboutButton->setText(QApplication::translate("Form", "About", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        debugButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        debugButton->setText(QApplication::translate("Form", "Debug", 0, QApplication::UnicodeUTF8));
        debugWidget->setWindowTitle(QString());
        camera->setText(QString());
        colorButton->setText(QApplication::translate("Form", "Color", 0, QApplication::UnicodeUTF8));
        strategyBox->clear();
        strategyBox->insertItems(0, QStringList()
         << QApplication::translate("Form", "All pixels", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Form", "Max square", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Form", "Fit line", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEBCAMWIZARD_H
