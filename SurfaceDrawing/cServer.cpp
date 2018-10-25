#include "cServer.h"

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
    auto port = serverPort();
    auto port2 = serverPort();
}


void
cServer::Run()
{
    if( !listen() )
        qDebug() << "Error";

    qDebug() << "Listening on port :  " << serverPort();
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
    mClients.push_back( nextPendingConnection() );
    qDebug() << "New client connected";
}



