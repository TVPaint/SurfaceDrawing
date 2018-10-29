#pragma once

#include "cUser.h"

#include <QDataStream>

#include <QTcpSocket>


class cPaperLogic;

class cClient :
    public QTcpSocket
{
    Q_OBJECT

public:
    enum eDataReadingState
    {
        kNone,
        kGRID,
        kSIMPLE,
        kACTION
    };

public:
    ~cClient();
    cClient();

public:
    void  AskConnection();

    void SendNewDirection( int iDirection );
    void SendRespawnRequest();

public slots:
    void  Connected();
    void  ConnectionError( QAbstractSocket::SocketError );
    void  GetData();


signals:
    void  newUserArrived( cUser* iUser );
    void  myUserAssigned( cUser* iUser );
    void  paperLogicArrived( cPaperLogic& iPaper );

    void  userChangedDirection( cUser* iUser );
    void  userRequestedRespawn( cUser* iUser );

private:
    eDataReadingState   mDataReadingState = kNone;
    bool                mConnectedToServer = false;
    QDataStream         mDataStream;
};

