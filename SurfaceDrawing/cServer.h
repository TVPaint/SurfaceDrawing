#pragma once

#include  "cPaperLogic.h"

#include <QDataStream>
#include <QTextStream>
#include <QFile>
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

    enum ePacketType : quint8
    {
        kGrid = 0,
        kSimple,
        kAction,
        kClock,
        kDisc,
        kPong,
        kSnap
    };



public:
    ~cServer();
    cServer();

public:
    void  Run();


    void  SendGridToAllClient();
    void  SendNextSnapShotToAllClient();
    void  SendClockToAllClients();
    void  SendSimpleUserPositionToClient( QTcpSocket* iClient, cUser* iUser, eType iType );
    void  SendUserActionToClient( QTcpSocket* iClient, cUser* iUser, int iAction );
    void  SendUserDisconnectedToAllClients( int iIndex );

    void  SendPongToClient( QTcpSocket* iClient );

    void  BuildPacket( QByteArray* oData, QDataStream* oStream, int iType );


    bool  ReadUserAction( int iClientIndex, int iAction );

public slots:
    void  NewClientConnected();
    void  ClientDisconnected();

    void  Update();
    void  GetData();

signals:
    void  quit();


private:
    // LOGS
    void  _LOG( const QString& iText );
    bool  _CheckForData();
    bool  _IsClientReady( int iIndex ) const;


private:
    QMap< int, QTcpSocket* >    mClients;
    QMap< int, QDataStream* >   mDataStream;
    QVector< int >              mReadyClients;

    cPaperLogic*                mPaperLogic;
    QTimer*                     mApplicationClock;
    QTimer*                     mUpdateTimer;
    quint64                     mPreviousTime;
    quint64                     mTimeBuffer = 0;

    quint64                     mPreviousSnapTickSent = -1;

    bool                        mQuit = false;


    // DEBUG
    QFile*                       mDEBUGFile;
    QTextStream*                 mDEBUGStream;
};

