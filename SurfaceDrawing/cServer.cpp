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
    if( !listen() )
        qDebug() << "Error";

    qDebug() << "Listening on port :  " << serverPort();
}


void
cServer::SendDataToAllClients( const QString & iData )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );

    stream << iData;

    for( auto client : mClients )
    {
        client->write( data );
    }
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

    mClients.push_back( nextPendingConnection() );

    auto newUser = new cUser( mPaperLogic->mAllUsers.size() );
    mPaperLogic->AddUser( newUser );

    SendDataToAllClients( QString::number( newUser->mIndex ) + "-" + QString::number( newUser->mPosition.x() ) + "," + QString::number( newUser->mPosition.y() ) );
}



