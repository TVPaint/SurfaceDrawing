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
    connect( this, &QTcpSocket::hostFound, this, &cClient::HostFound );

    connect( this, SLOT( error(QAbstractSocket::SocketError)), this, SIGNAL(ConnectionError(QAbstractSocket::SocketError)) );
}


void
cClient::AskConnection()
{
    cConnectionDialog cdial( "LocalHost" );
    if( cdial.exec() )
    {
        connectToHost( cdial.GetIP(), cdial.GetPort() );
        if( !waitForConnected( 5000 ) )
        {
            qDebug() << "TimedOut " << errorString();
        }
    }

}


void
cClient::HostFound()
{
    int a = 0;
    qDebug() << "Connected to server";
}


void
cClient::ConnectionError( QAbstractSocket::SocketError iError )
{
    qDebug() << "Connection Error : " << iError;
}


void
cClient::Connected()
{
    int a = 0;
    qDebug() << "Connected to server";
}



