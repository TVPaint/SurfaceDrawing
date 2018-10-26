#include "cClient.h"


#include "cConnectionDialog.h"

#include <QHostAddress>



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
cClient::ReadNewUser( const QString & iUserSerialized )
{
    QStringList package = iUserSerialized.split( "-" );

    QString type = package[ 0 ];
    int index = package[ 1 ].toInt();
    QStringList position = package[ 2 ].split( "," );

    QPoint userPos( position[0].toInt(), position[1].toInt() );

    auto  newUser = new cUser( index );
    newUser->setPosition( userPos );

    if( type == "assign" )
    {
        qDebug() << "MyUser is : " + QString::number( index ) + " at : " + QString::number( userPos.x() ) + "-" + QString::number( userPos.y() );
        emit myUserAssigned( newUser );
    }
    else
    {
        qDebug() << "OtherUser is : " + QString::number( index ) + " at : " + QString::number( userPos.x() ) + "-" + QString::number( userPos.y() );
        emit newUserArrived( newUser );
    }
}


void
cClient::ConnectionError( QAbstractSocket::SocketError iError )
{
    qDebug() << "Connection Error : " << iError;
}


void
cClient::GetData()
{
    mDataStream.startTransaction();

    QString dataString;
    mDataStream >> dataString;

    if( !mDataStream.commitTransaction() ) // If packet isn't complete, this will restore data to initial position, so we can read again on next GetData
        return;

    ReadNewUser( dataString );
}


void
cClient::Connected()
{
    mConnectedToServer = true;
    qDebug() << "Connected to server";
}



