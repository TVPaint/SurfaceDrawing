#pragma once

#include "cUser.h"

#include <QDataStream>

#include <QTcpSocket>
#include <QTimer>

#include <chrono>


class cPaperLogic;
class cSnapShot;

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
        kPONG,
        KSNAP
    };

public:
    ~cClient();
    cClient();

public:
    void  AskConnection();

    void SendNewDirection( quint64 iTick, int iDirection );
    void SendCompUse( quint64 iTick, int iComp );
    void SendCompStopUse( quint64 iTick, int iComp );
    void SendRespawnRequest();
    void SendRdy(); // Client is properly set and connected, and now ready to get data;

    void  SendPing();
    void  SendStopResync();
    void  StartPingAveraging();

    quint64  GetTime();
    quint64  GetLatencyInMsFromTimestamp( quint64 iTimestamp );

public slots:
    void  Connected();
    void  ConnectionError( QAbstractSocket::SocketError );
    void  GetData();


signals:
    void  newUserArrived( cUser* iUser );
    void  myUserAssigned( cUser* iUser );
    void  paperLogicArrived( cPaperLogic& iPaper, int  iLatencyInMs );
    void  snapShotArrived( cSnapShot* iSnapShot );

    void  userChangedDirection( cUser* iUser, quint64 iTick );
    void  userRequestedRespawn( cUser* iUser );

    void  userDisconnected( int iIndex );


public:
    void  _LOG( const QString& iText );
private:
    void  _PingAveraging( qint64 timestamp );

public:
    eDataReadingState   mDataReadingState = kNone;
    bool                mConnectedToServer = false;
    QDataStream         mDataStream;

    QTimer*             mApplicationClock;
    int                 mClockOffset;           // Offset with server, as we can't restart the clock once started (because start will round given time to the second ... )

    quint64             mPingStartTime;
    bool                mPingAveraging;         // In the state where client is pinging to calculate a speed
    int                 mPingAveragingCounter;  // To count where we at while receiving packets
    int                 mPingAverage;

    // DEBUG
    QFile*                       mDEBUGFile     = 0;
    QTextStream*                 mDEBUGStream   = 0;
};

