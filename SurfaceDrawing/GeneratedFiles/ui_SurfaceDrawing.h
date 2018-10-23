/********************************************************************************
** Form generated from reading UI file 'SurfaceDrawing.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SURFACEDRAWING_H
#define UI_SURFACEDRAWING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SurfaceDrawingClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SurfaceDrawingClass)
    {
        if (SurfaceDrawingClass->objectName().isEmpty())
            SurfaceDrawingClass->setObjectName(QStringLiteral("SurfaceDrawingClass"));
        SurfaceDrawingClass->resize(600, 400);
        menuBar = new QMenuBar(SurfaceDrawingClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        SurfaceDrawingClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SurfaceDrawingClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SurfaceDrawingClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(SurfaceDrawingClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SurfaceDrawingClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SurfaceDrawingClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SurfaceDrawingClass->setStatusBar(statusBar);

        retranslateUi(SurfaceDrawingClass);

        QMetaObject::connectSlotsByName(SurfaceDrawingClass);
    } // setupUi

    void retranslateUi(QMainWindow *SurfaceDrawingClass)
    {
        SurfaceDrawingClass->setWindowTitle(QApplication::translate("SurfaceDrawingClass", "SurfaceDrawing", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SurfaceDrawingClass: public Ui_SurfaceDrawingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SURFACEDRAWING_H
