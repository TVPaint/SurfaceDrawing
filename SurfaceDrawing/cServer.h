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
        kPong
    };



public:
    ~cServer();
    cServer();

public:
    void  Run();


    void  SendGridToAllClient();
    void  SendClockToAllClients();
    void  SendSimpleUserPositionToClient( QTcpSocket* iClient, cUser* iUser, eType iType );
    void  SendUserActionToClient( QTcpSocket* iClient, cUser* iUser, int iAction );
    void  SendUserDisconnectedToAllClients( int iIndex );

    void  SendPongToClient( QTcpSocket* iClient );

    void  BuildPacket( QByteArray* oData, QDataStream* oStream, int iType );

public slots:
    void  NewClientConnected();
    void  ClientDisconnected();

    void  Update();
    void  NetworkTick();
    void  GetData();

signals:
    void  quit();


private:
    // LOGS
    void  _LOG( const QString& iText );


private:
    QMap< int, QTcpSocket* >    mClients;
    QMap< int, QDataStream* >   mDataStream;

    cPaperLogic*                mPaperLogic;
    QTimer*                     mApplicationClock;
    QTimer*                     mUpdateTimer;
    quint64                     mPreviousTime;
    quint64                     mTimeBuffer = 0;

    bool                        mQuit = false;


    // DEBUG
    QFile*                       mDEBUGFile;
    QTextStream*                 mDEBUGStream;
};

