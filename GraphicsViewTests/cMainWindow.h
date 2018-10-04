#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cMainWindow.h"

#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>

class cCustomGraphicsView;

class cMainWindow :
    public QMainWindow
{
    Q_OBJECT

public:
    cMainWindow(QWidget *parent = Q_NULLPTR);

public:
    void TimerTick();

private:
    Ui::cMainWindow     ui;

    QGraphicsScene*     mScene;

    int                 mCurrentFrame;
    QTimer*             mAnimationTimer;
};
