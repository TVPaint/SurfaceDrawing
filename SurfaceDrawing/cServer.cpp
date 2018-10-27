#include "cServer.h"

#include "cUser.h"

#include <QDataStream>
#include <QHostAddress>



cServer::~cServer()
{
    delete  mTimer;
    delete  mPaperLogic;
}


cServer::cServer() :
    QTcpServer()
{
    mTimer = new  QTimer();
    mTimer->start( 1000/60 );

    connect( mTimer, &QTimer::timeout, this, &cServer::Update );
    connect( this, &QTcpServer::newConnection, this, &cServer::NewClientConnected );

    mPaperLogic = new cPaperLogic();
    mPaperLogic->Init();
}


void
cServer::Run()
{
    if( !listen( QHostAddress::Any, 55666 ) )
        qDebug() << "Error";

    qDebug() << "Listening on port :  " << serverPort();
}


void
cServer::SendDataToAllClients( const QString & iData )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );

    stream << *mPaperLogic;

    for( auto client : mClients )
    {
        if( !client->write( data ) )
            qDebug() << "DataWritting failed";
    }
}


void
cServer::SendDataToSpecificClients( const QString & iData, QTcpSocket * iClient )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );
    stream.setDevice( iClient );

    stream << iData;
}


void
cServer::SendGridToClient( QTcpSocket * iClient )
{
    QByteArray data;

        QDataStream stream( &data, QIODevice::WriteOnly );
        stream.setVersion( QDataStream::Qt_5_10 );
        stream.setDevice( iClient );

        stream << char(0);
        stream << *mPaperLogic;
}


void
cServer::SendSimpleUserPositionToClient( QTcpSocket * iClient, cUser* iUser )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );
    stream.setDevice( iClient);

    stream << char(1);
    stream << QString( "other-" + QString::number( iUser->mIndex ) + "-" + QString::number( iUser->mPosition.x() ) + "," + QString::number( iUser->mPosition.y() ) );
}


void
cServer::Update()
{
    mPaperLogic->Update();

    if( mQuit )
        emit quit();
}


void
cServer::NewClientConnected( )
{
    qDebug() << "New client connected";

    auto newUser = new cUser( mPaperLogic->mAllUsers.size() );
    mPaperLogic->AddUser( newUser );

    auto it = mClients.insert( newUser->mIndex, nextPendingConnection() );
    auto newClient = it.value();
    SendGridToClient( newClient );
    SendGridToClient( newClient );
    //SendSimpleUserPositionToClient( newClient, newUser );

    // Telling clients a new one arrived
    //SendDataToAllClients( "other-" + QString::number( newUser->mIndex ) + "-" + QString::number( newUser->mPosition.x() ) + "," + QString::number( newUser->mPosition.y() ) );


    //// Getting the new client
    //auto it = mClients.insert( newUser->mIndex, nextPendingConnection() );
    //auto newClient = it.value();

    //// Assigning him its user
    //SendDataToSpecificClients( "assign-" + QString::number( newUser->mIndex ) + "-" + QString::number( newUser->mPosition.x() ) + "," + QString::number( newUser->mPosition.y() ), newClient );

    //// Send other users to new client
    //for( auto client : mClients )
    //{
    //    if( client == newClient ) // Don't send you own  user as an "enemy" user
    //        continue;

    //    auto user = mPaperLogic->mAllUsers[ mClients.key( client ) ];
    //    SendDataToSpecificClients( "other-" + QString::number( user->mIndex ) + "-" + QString::number( user->mPosition.x() ) + "," + QString::number( user->mPosition.y() ), newClient );
    //}
}



