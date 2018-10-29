#include "cClient.h"

#include "cServer.h"

#include "cConnectionDialog.h"
#include "cPaperLogic.h"

#include <QHostAddress>

#include <iostream>

cClient::~cClient()
{
}


cClient::cClient() :
    QTcpSocket()
{
    connect( this, &QTcpSocket::connected, this, &cClient::Connected );
    connect( this, &QTcpSocket::readyRead, this, &cClient::GetData );

    connect( this, SLOT( error(QAbstractSocket::SocketError)), this, SIGNAL(ConnectionError(QAbstractSocket::SocketError)) );

    mDataStream.setDevice( this );
    mDataStream.setVersion( QDataStream::Qt_5_10 );
}


void
cClient::AskConnection()
{
    cConnectionDialog cdial( "LocalHost" );
    while( !mConnectedToServer )
    {
        if( cdial.exec() )
        {
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
cClient::ConnectionError( QAbstractSocket::SocketError iError )
{
    qDebug() << "Connection Error : " << iError;
}


void
cClient::GetData()
{
    if( mDataReadingState == kNone )
    {
        quint8 header;

        mDataStream.startTransaction();

        mDataStream >> header;

        if( !mDataStream.commitTransaction() ) // If packet isn't complete, this will restore data to initial position, so we can read again on next GetData
            return;

        if( header == 0 )
            mDataReadingState = kGRID;
        else if( header == 1 )
            mDataReadingState = kSIMPLE;
    }

    if( mDataReadingState == kGRID )
    {
        QByteArray dataCompressed;


        mDataStream.startTransaction();
        mDataStream >> dataCompressed;
        if( !mDataStream.commitTransaction() )
            return;

        cPaperLogic data;
        dataCompressed = qUncompress( dataCompressed );
        QDataStream streamTemp( &dataCompressed, QIODevice::ReadOnly );
        streamTemp >> data;

        emit  paperLogicArrived( data );

        mDataReadingState = kNone;
    }
    else if( mDataReadingState == kSIMPLE )
    {
        int type;
        cUser* newUser = new cUser( -1, Qt::transparent );

        mDataStream.startTransaction();
        mDataStream >> type;
        mDataStream >> *newUser;

        if( !mDataStream.commitTransaction() )
            return;

        auto ttttype = cServer::eType( type );
        if( ttttype == cServer::kSelfUser )
            emit myUserAssigned( newUser );
        else
            emit newUserArrived( newUser );

        mDataReadingState = kNone;
    }
}


void
cClient::Connected()
{
    mConnectedToServer = true;
    qDebug() << "Connected to server";
}



