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
    void  SendSimpleUserPositionToClient( QTcpSocket* iClient, cUser* iUser, eType iType );
    void  SendUserActionToClient( QTcpSocket* iClient, cUser* iUser, int iAction );

public slots:
    void  NewClientConnected();
    void  Update();
    void  NetworkTick();
    void  GetData();

signals:
    void  quit();

private:
    cPaperLogic*                mPaperLogic;
    QMap< int, QTcpSocket* >    mClients;
    QTimer*                     mUpdateTimer;
    QTimer*                     mPacketTimer;
    QMap< int, QDataStream* >   mDataStream;

    bool                        mQuit = false;
};

