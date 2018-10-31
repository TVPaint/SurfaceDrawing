#pragma once

#include  "cPaperLogic.h"

#include <QDataStream>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

class cServer :
    public QTcpServer
{
    Q_OBJECT


public:
    enum eType
    {
        kOtherUser,
        kSelfUser
    };

public:
    ~cServer();
    cServer();

public:
    void  Run();


    void  SendGridToAllClient();
    void  SendClockToAllClients();
    void  SendGridToClient( QTcpSocket* iClient );
    void  SendSimpleUserPositionToClient( QTcpSocket* iClient, cUser* iUser, eType iType );
    void  SendUserActionToClient( QTcpSocket* iClient, cUser* iUser, int iAction );


public slots:
    void  NewClientConnected();
    void  ClientDisconnected();

    void  Update();
    void  NetworkTick();
    void  GetData();

signals:
    void  quit();

private:
    QMap< int, QTcpSocket* >    mClients;
    QMap< int, QDataStream* >   mDataStream;

    cPaperLogic*                mPaperLogic;
    QTimer*                     mApplicationTimer;
    QTimer*                     mUpdateTimer;
    quint64                     mPreviousTime;
    quint64                     mTimeBuffer = 0;

    bool                        mQuit = false;

};

