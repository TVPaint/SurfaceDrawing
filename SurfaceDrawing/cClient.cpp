#include "cClient.h"

#include "cServer.h"

#include "cConnectionDialog.h"
#include "cPaperLogic.h"

#include <QHostAddress>

#include <iostream>
#include <ctime>


#define  PINGAVERAGINGAMOUNT 10



cClient::~cClient()
{
    delete  mApplicationClock;
}


cClient::cClient() :
    QTcpSocket()
{
    connect( this, &QTcpSocket::connected, this, &cClient::Connected );
    connect( this, &QTcpSocket::readyRead, this, &cClient::GetData );

    mDataStream.setDevice( this );
    mDataStream.setVersion( QDataStream::Qt_5_10 );

    mApplicationClock = new QTimer();
    mApplicationClock->start( CLOCKTIME );
    mClockOffset = -1;
}


void
cClient::AskConnection()
{
    QFile recentFile( "recentHosts.txt" );
    QTextStream stream( &recentFile );
    QString  lastIP = "LocalHost";
    if( recentFile.open( QIODevice::ReadOnly ) )
    {
        stream >> lastIP;
        recentFile.close();
    }

    cConnectionDialog cdial( lastIP );
    while( !mConnectedToServer )
    {
        if( cdial.exec() )
        {
            auto ip = cdial.GetIP();
            if( recentFile.open( QIODevice::WriteOnly ) )
            {
                stream << ip;
                recentFile.close();
            }

            connectToHost( cdial.GetIP(), cdial.GetPort() );
            if( !waitForConnected( 5000 ) )
                qDebug() << "TimedOut " << errorString();
        }
    }
}


void
cClient::SendNewDirection( quint64 iTick, int iDirection )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );

    stream << int( iDirection );
    stream << iTick;

    write( data );
}


void
cClient::SendRespawnRequest()
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );

    stream << int( 10 );
    write( data );
}


void
cClient::SendRdy()
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );

    stream << int( 666 );
    write( data );
}


void
cClient::SendPing()
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );

    stream << int( 99 );
    write( data );
    mPingStartTime = mApplicationClock->remainingTimeAsDuration().count();
}


void
cClient::StartPingAveraging()
{
    _LOG( "Starting ping averaging ..." );
    mPingAveraging = true;
    mPingAveragingCounter = 0;
    mPingAverage = 0;
    SendPing(); // only one, each ping has to be sent after previous one is answered, otherwise it'll pack all ping requests in one packet
}


quint64
cClient::GetTime()
{
    return  mApplicationClock->remainingTime() - mClockOffset;
}


quint64
cClient::GetLatencyInMsFromTimestamp( quint64 iTimestamp )
{
    return  iTimestamp - GetTime();
}


void
cClient::ConnectionError( QAbstractSocket::SocketError iError )
{
    qDebug() << "Connection Error : " << iError;
}


void
cClient::GetData()
{
    qint64      timestamp;
    quint64     packetTick = -1;

    _LOG( "======================INC================= : " + QString::number( bytesAvailable() ) );

    while( bytesAvailable() > 0 )
    {
        if( mDataReadingState == kNone )
        {
            mDataStream.startTransaction();
            mDataStream >> timestamp;
            if( !mDataStream.commitTransaction() ) // If packet isn't complete, this will restore data to initial position, so we can read again on next GetData
                return;

            //_LOG( "TIMESTAMP : " + QString::number( timestamp) );
            _LOG( "PACKET got sent in : " + QString::number( qint64(timestamp - GetTime()) ) );

            mDataStream.startTransaction();
            mDataStream >> packetTick;
            if( !mDataStream.commitTransaction() ) // If packet isn't complete, this will restore data to initial position, so we can read again on next GetData
                return;

            _LOG( "At tick : " + QString::number( packetTick ) );


            quint8 header;
            mDataStream.startTransaction();
            mDataStream >> header;
            if( !mDataStream.commitTransaction() ) // If packet isn't complete, this will restore data to initial position, so we can read again on next GetData
                return;

            if( header == cServer::kGrid )
                mDataReadingState = kGRID;
            else if( header == cServer::kSimple )
                mDataReadingState = kSIMPLE;
            else if( header == cServer::kAction )
                mDataReadingState = kACTION;
            else if( header == cServer::kClock )
                mDataReadingState = kCLOCK;
            else if( header == cServer::kDisc )
                mDataReadingState = kDISC;
            else if( header == cServer::kPong )
                mDataReadingState = kPONG;
            else if( header == cServer::kSnap )
                mDataReadingState = KSNAP;
        }

        if( mDataReadingState == kGRID )
        {
            _LOG( "Data type : GRID" );

            cPaperLogic data;
            mDataStream.startTransaction();
            mDataStream >> data;
            if( !mDataStream.commitTransaction() )
                return;

            _LOG("INC Grid has " + QString::number( data.mAllUsers.size() ) );
            emit  paperLogicArrived( data, GetLatencyInMsFromTimestamp( timestamp ) );

            mDataReadingState = kNone;
        }
        else if( mDataReadingState == kSIMPLE )
        {
            _LOG( "Data type : SIMPLE" );

            int type;
            cUser* newUser = new cUser( -1, Qt::transparent );

            mDataStream.startTransaction();
            mDataStream >> type;
            mDataStream >> *newUser;

            if( !mDataStream.commitTransaction() )
                return;

            auto typeAsEnum = cServer::eType( type );
            if( typeAsEnum == cServer::kSelfUser )
            {
                qDebug() << "SELF" << newUser->mGUIPosition;
                _LOG( "SELF : " + QString::number( newUser->mGUIPosition.x() ) + "-" + QString::number( newUser->mGUIPosition.y() ) );

                emit myUserAssigned( newUser );
            }
            else
            {
                _LOG( "OTHER : " + QString::number( newUser->mGUIPosition.x() ) + "-" + QString::number( newUser->mGUIPosition.y() ) );

                emit newUserArrived( newUser );
            }

            mDataReadingState = kNone;
        }
        else if( mDataReadingState == kACTION )
        {
            _LOG( "Data type : ACTION" );

            int action;
            cUser* newUser = new cUser( -1, Qt::transparent );

            mDataStream.startTransaction();
            mDataStream >> action;
            mDataStream >> *newUser;

            if( !mDataStream.commitTransaction() )
                return;

            _LOG( "User : " + QString::number( newUser->mIndex ) + " did an action " + QString::number( action ) );

            switch( action )
            {
                case 1: // Right
                    newUser->setMovementVector( QPoint( -1, 0 ) );
                    emit  userChangedDirection( newUser, packetTick );
                    break;
                case 2: // Left
                    newUser->setMovementVector( QPoint( 1, 0 ) );
                    emit  userChangedDirection( newUser, packetTick );
                    break;
                case 3: // Top
                    newUser->setMovementVector( QPoint( 0, -1 ) );
                    emit  userChangedDirection( newUser, packetTick );
                    break;
                case 4: // Bottom
                    newUser->setMovementVector( QPoint( 0, 1 ) );
                    emit  userChangedDirection( newUser, packetTick );
                    break;

                case 10 : // Respawn
                    emit userRequestedRespawn( newUser );
                    break;

                default:
                    break;
            }

            delete  newUser;
            mDataReadingState = kNone;
        }
        else if( mDataReadingState == kCLOCK )
        {
            _LOG( "Data type : CLOCK" );
            StartPingAveraging();

            mDataReadingState = kNone;
        }
        else if( mDataReadingState == kDISC )
        {
            _LOG( "Data type : DISC" );

            int index;

            mDataStream.startTransaction();
            mDataStream >> index;
            if( !mDataStream.commitTransaction() )
                return;

            emit userDisconnected( index );

            mDataReadingState = kNone;
        }
        else if( mDataReadingState == kPONG )
        {
            auto ping = mPingStartTime - mApplicationClock->remainingTimeAsDuration().count();

            _LOG( "PING : " + QString::number( mPingStartTime - mApplicationClock->remainingTime() ) + " ms" );
            _PingAveraging( timestamp );

            mDataReadingState = kNone;
        }
        else if( mDataReadingState == KSNAP )
        {
            _LOG( "Data type : SNAP" );

            cSnapShot* ss = new cSnapShot( packetTick );

            mDataStream.startTransaction();
            mDataStream >> *ss;
            if( !mDataStream.commitTransaction() )
                return;

            emit snapShotArrived( ss );

            mDataReadingState = kNone;
        }
    }
}


void
cClient::_LOG( const QString & iText )
{
    qDebug() << mApplicationClock->remainingTimeAsDuration().count() << " : " << iText;

}


void
cClient::_PingAveraging( qint64 timestamp )
{
    if( mClockOffset == -1 && mPingAveraging && mPingAveragingCounter <= PINGAVERAGINGAMOUNT )
    {
        _LOG( "Undergoing ping averaging ..." );
        ++mPingAveragingCounter;
        mPingAverage += mPingStartTime - mApplicationClock->remainingTimeAsDuration().count();
        SendPing();

        if( mPingAveragingCounter >= PINGAVERAGINGAMOUNT )
        {
            int pingAvg = mPingAverage/mPingAveragingCounter;

            _LOG( "Ping average : " + QString::number( pingAvg ) );
            mPingAveraging = false;
            mClockOffset = std::abs( int(timestamp - mApplicationClock->remainingTime()) ) - pingAvg/2;

            _LOG( "Offset : " + QString::number( mClockOffset ) );
            _LOG( "Timestamp : " + QString::number( timestamp ) );
            _LOG( "Clock : " + QString::number( GetTime() ) );

            SendPing(); // to check clock diff
            SendRdy();
        }
    }
}


void
cClient::Connected()
{
    mConnectedToServer = true;
    _LOG( "Connected to server" );
}



