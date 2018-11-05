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
        kCLOCK,
        kDISC,
        kPONG
    };

public:
    ~cClient();
    cClient();

public:
    void  AskConnection();

    void SendNewDirection( int iDirection );
    void SendRespawnRequest();

    void  SendPing();
    void  StartPingAveraging();

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

    void  userDisconnected( int iIndex );

private:
    void  _LOG( const QString& iText );

public:
    eDataReadingState   mDataReadingState = kNone;
    bool                mConnectedToServer = false;
    QDataStream         mDataStream;

    QTimer*             mApplicationClock;
    quint64             mPingStartTime;
    bool                mPingAveraging; // In the state where client is pinging to calculate a speed
    int                 mPingAveragingCounter; // To count where we at while receiving packets
    int                 mPingAverage;
};

