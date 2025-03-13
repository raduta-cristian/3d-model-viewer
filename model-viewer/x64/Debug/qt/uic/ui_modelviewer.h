/********************************************************************************
** Form generated from reading UI file 'modelviewer.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELVIEWER_H
#define UI_MODELVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_modelviewerClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *modelviewerClass)
    {
        if (modelviewerClass->objectName().isEmpty())
            modelviewerClass->setObjectName("modelviewerClass");
        modelviewerClass->resize(600, 400);
        menuBar = new QMenuBar(modelviewerClass);
        menuBar->setObjectName("menuBar");
        modelviewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(modelviewerClass);
        mainToolBar->setObjectName("mainToolBar");
        modelviewerClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(modelviewerClass);
        centralWidget->setObjectName("centralWidget");
        modelviewerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(modelviewerClass);
        statusBar->setObjectName("statusBar");
        modelviewerClass->setStatusBar(statusBar);

        retranslateUi(modelviewerClass);

        QMetaObject::connectSlotsByName(modelviewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *modelviewerClass)
    {
        modelviewerClass->setWindowTitle(QCoreApplication::translate("modelviewerClass", "modelviewer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class modelviewerClass: public Ui_modelviewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELVIEWER_H
