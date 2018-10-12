#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cMainWindow.h"

#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include <QDataWidgetMapper>

#include "ToolModel.h"

class cCustomGraphicsView;

class cMainWindow :
    public QMainWindow
{
    Q_OBJECT

public:
    cMainWindow(QWidget *parent = Q_NULLPTR);

public:
    void TimerTick();
    void PlayPressed();
    void StopPressed();

    void CurrentFrameChanged( int iNewIndex );

    void UpdateColor();
    void AskColor();

signals:
    void  currentFrameChangeAsked( int iCurrentFrame );


private:
    Ui::cMainWindow     ui;

    QGraphicsScene*     mScene;

    int                 mCurrentFrame;
    QTimer*             mAnimationTimer;

    QDataWidgetMapper*  mMapper;
    cToolModel*         mToolModel;

};
