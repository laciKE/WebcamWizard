/********************************************************************************
** Form generated from reading UI file 'webcamWizard.ui'
**
** Created: Thu Jun 21 22:49:33 2012
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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QPushButton *pushButton;
    QLabel *desktop;
    QTextEdit *debugOutput;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(1000, 740);
        pushButton = new QPushButton(Form);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(220, 10, 181, 31));
        desktop = new QLabel(Form);
        desktop->setObjectName(QString::fromUtf8("desktop"));
        desktop->setGeometry(QRect(40, 60, 800, 600));
        desktop->setLineWidth(1);
        debugOutput = new QTextEdit(Form);
        debugOutput->setObjectName(QString::fromUtf8("debugOutput"));
        debugOutput->setGeometry(QRect(680, 40, 311, 371));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Form", "Calibration - initialize", 0, QApplication::UnicodeUTF8));
        desktop->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEBCAMWIZARD_H
