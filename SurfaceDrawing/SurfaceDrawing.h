#pragma once

#include "cPaperLogic.h"


#include "cUser.h"
#include "cCanvas.h"


#include <QMainWindow>
#include <QBoxLayout>


#include "ui_SurfaceDrawing.h"



class SurfaceDrawing :
    public QMainWindow
{
    Q_OBJECT

public:
    ~SurfaceDrawing();
    SurfaceDrawing(QWidget *parent = Q_NULLPTR);

public:
    void  Init();
    void  Update();

private:
    Ui::SurfaceDrawingClass     ui;
    cPaperLogic*                mPaperLogic;

    QVector< cUser* >           mAllUsers;

    // GUI
    QVBoxLayout*    mMainLayout;
    cCanvas*        mCanvas;
};
