/********************************************************************************
** Form generated from reading UI file 'cMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CMAINWINDOW_H
#define UI_CMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Canvas.h"
#include "cCustomGraphicsView.h"

QT_BEGIN_NAMESPACE

class Ui_cMainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QLabel *previewLabel;
    QHBoxLayout *horizontalLayout_2;
    QFormLayout *formLayout;
    QLabel *penSizeLabel;
    QSpinBox *penSizeSpinBox;
    cCanvas *canvas;
    QHBoxLayout *horizontalLayout;
    QPushButton *playButton;
    QPushButton *stopButton;
    cCustomGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *cMainWindow)
    {
        if (cMainWindow->objectName().isEmpty())
            cMainWindow->setObjectName(QStringLiteral("cMainWindow"));
        cMainWindow->resize(649, 589);
        centralWidget = new QWidget(cMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        previewLabel = new QLabel(centralWidget);
        previewLabel->setObjectName(QStringLiteral("previewLabel"));
        previewLabel->setMinimumSize(QSize(0, 0));
        previewLabel->setMaximumSize(QSize(16777215, 16777215));
        previewLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(previewLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        penSizeLabel = new QLabel(centralWidget);
        penSizeLabel->setObjectName(QStringLiteral("penSizeLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, penSizeLabel);

        penSizeSpinBox = new QSpinBox(centralWidget);
        penSizeSpinBox->setObjectName(QStringLiteral("penSizeSpinBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, penSizeSpinBox);


        horizontalLayout_2->addLayout(formLayout);

        canvas = new cCanvas(centralWidget);
        canvas->setObjectName(QStringLiteral("canvas"));

        horizontalLayout_2->addWidget(canvas);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        playButton = new QPushButton(centralWidget);
        playButton->setObjectName(QStringLiteral("playButton"));

        horizontalLayout->addWidget(playButton);

        stopButton = new QPushButton(centralWidget);
        stopButton->setObjectName(QStringLiteral("stopButton"));

        horizontalLayout->addWidget(stopButton);


        verticalLayout->addLayout(horizontalLayout);

        graphicsView = new cCustomGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setMaximumSize(QSize(16777215, 200));

        verticalLayout->addWidget(graphicsView);

        cMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(cMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 649, 21));
        cMainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(cMainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        cMainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(cMainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        cMainWindow->setStatusBar(statusBar);

        retranslateUi(cMainWindow);

        QMetaObject::connectSlotsByName(cMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *cMainWindow)
    {
        cMainWindow->setWindowTitle(QApplication::translate("cMainWindow", "cMainWindow", nullptr));
        previewLabel->setText(QApplication::translate("cMainWindow", "PREVIEW", nullptr));
        penSizeLabel->setText(QApplication::translate("cMainWindow", "PenSize", nullptr));
        playButton->setText(QApplication::translate("cMainWindow", "Play", nullptr));
        stopButton->setText(QApplication::translate("cMainWindow", "Stop", nullptr));
    } // retranslateUi

};

namespace Ui {
    class cMainWindow: public Ui_cMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMAINWINDOW_H
