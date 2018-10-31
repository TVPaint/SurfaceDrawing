#pragma once

#include "cUser.h"

#include <QDataStream>

#include <QTcpSocket>
#include <QTimer>


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
        kACTION,
        kCLOCK
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
    void  paperLogicArrived( cPaperLogic& iPaper, quint64 iTimeStamp );

    void  userChangedDirection( cUser* iUser );
    void  userRequestedRespawn( cUser* iUser );

public:
    eDataReadingState   mDataReadingState = kNone;
    bool                mConnectedToServer = false;
    QDataStream         mDataStream;

    QTimer*             mApplicationClock;
};

