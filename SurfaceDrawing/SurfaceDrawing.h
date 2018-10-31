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


public slots:
    void  RespawnRequest();
    void  DirectionChanged( int iDirection );

    void  PaperLogicArrived( cPaperLogic& iPaper, quint64 iTimestamp );
    void  NewUserArrived( cUser* iUser );
    void  MyUserAssigned( cUser* iUser );
    void  UserDirectionChanged( cUser* iUser );
    void  UserRequestedRespawn( cUser* iUser );



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
