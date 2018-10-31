#include "cClient.h"

#include "cServer.h"

#include "cConnectionDialog.h"
#include "cPaperLogic.h"

#include <QHostAddress>

#include <iostream>
#include <ctime>

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
cClient::SendNewDirection( int iDirection )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );

    stream << int( iDirection );
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
cClient::ConnectionError( QAbstractSocket::SocketError iError )
{
    qDebug() << "Connection Error : " << iError;
}


void
cClient::GetData()
{
    qint64 timestamp;

    while( bytesAvailable() > 0 )
    {
        if( mDataReadingState == kNone )
        {
            mDataStream.startTransaction();
            mDataStream >> timestamp;
            if( !mDataStream.commitTransaction() ) // If packet isn't complete, this will restore data to initial position, so we can read again on next GetData
                return;

            qDebug() << "PACKET got sent in : " << QString::number( timestamp - mApplicationClock->remainingTimeAsDuration().count() );


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
        }

        if( mDataReadingState == kGRID )
        {
            qDebug() << "GRID";

            cPaperLogic data;
            mDataStream.startTransaction();
            mDataStream >> data;
            if( !mDataStream.commitTransaction() )
                return;

            emit  paperLogicArrived( data, timestamp );

            mDataReadingState = kNone;
        }
        else if( mDataReadingState == kSIMPLE )
        {
            qDebug() << "SIMPLE";

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
                emit myUserAssigned( newUser );
            }
            else
            {
                qDebug() << "OTHER : " << newUser->mGUIPosition;
                emit newUserArrived( newUser );
            }

            mDataReadingState = kNone;
        }
        else if( mDataReadingState == kACTION )
        {
            qDebug() << "ACTION";

            int action;
            cUser* newUser = new cUser( -1, Qt::transparent );

            mDataStream.startTransaction();
            mDataStream >> action;
            mDataStream >> *newUser;

            if( !mDataStream.commitTransaction() )
                return;

            qDebug() << "User : " + QString::number( newUser->mIndex ) + " did an action " + QString::number( action );

            switch( action )
            {
                case 1: // Right
                    newUser->setMovementVector( QPoint( -1, 0 ) );
                    emit  userChangedDirection( newUser );
                    break;
                case 2: // Left
                    newUser->setMovementVector( QPoint( 1, 0 ) );
                    emit  userChangedDirection( newUser );
                    break;
                case 3: // Top
                    newUser->setMovementVector( QPoint( 0, -1 ) );
                    emit  userChangedDirection( newUser );
                    break;
                case 4: // Bottom
                    newUser->setMovementVector( QPoint( 0, 1 ) );
                    emit  userChangedDirection( newUser );
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
            qDebug() << "CLOCK";

            quint64 clock;

            mDataStream.startTransaction();
            mDataStream >> clock;
            if( !mDataStream.commitTransaction() )
                return;

            mApplicationClock->start( clock );
            SendPing();
            mDataReadingState = kNone;
        }
        else if( mDataReadingState == kDISC )
        {
            qDebug() << "DISC";

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
            qDebug() << "PING : " << mPingStartTime - mApplicationClock->remainingTimeAsDuration().count() << " ms";

            mDataReadingState = kNone;
        }
    }
}


void
cClient::Connected()
{
    mConnectedToServer = true;
    qDebug() << "Connected to server";
}



