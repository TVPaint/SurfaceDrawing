#pragma once

#include "cPaperLogic.h"


#include "cUser.h"
#include "cCanvas.h"


#include "cClient.h"

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

    void  Start();

private:
    Ui::SurfaceDrawingClass     ui;
    cPaperLogic*                mPaperLogic;
    QTimer*                     mTimer;

    QVector< cUser* >           mAllUsers;

    // GUI
    QVBoxLayout*    mMainLayout;
    cCanvas*        mCanvas;

    // NETWORK
    cClient*     mClientSocket;
};
