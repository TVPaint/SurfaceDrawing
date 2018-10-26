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
cClient::ConnectionError( QAbstractSocket::SocketError iError )
{
    qDebug() << "Connection Error : " << iError;
}


void
cClient::GetData()
{
    QDataStream data;
    data.startTransaction();

    QString dataString;
    data >> dataString;
    int bp = 0;
}


void
cClient::Connected()
{
    mConnectedToServer = true;
    qDebug() << "Connected to server";
}



