/********************************************************************************
** Form generated from reading UI file 'debugForm.ui'
**
** Created: Fri Jun 22 02:15:08 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEBUGFORM_H
#define UI_DEBUGFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DebugForm
{
public:
    QTextEdit *debugOutput;

    void setupUi(QWidget *DebugForm)
    {
        if (DebugForm->objectName().isEmpty())
            DebugForm->setObjectName(QString::fromUtf8("DebugForm"));
        DebugForm->resize(587, 331);
        debugOutput = new QTextEdit(DebugForm);
        debugOutput->setObjectName(QString::fromUtf8("debugOutput"));
        debugOutput->setGeometry(QRect(360, 20, 211, 281));

        retranslateUi(DebugForm);

        QMetaObject::connectSlotsByName(DebugForm);
    } // setupUi

    void retranslateUi(QWidget *DebugForm)
    {
        DebugForm->setWindowTitle(QApplication::translate("DebugForm", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DebugForm: public Ui_DebugForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUGFORM_H
