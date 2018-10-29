#include "cServer.h"

#include "cUser.h"

#include <QHostAddress>


cServer::~cServer()
{
    delete  mUpdateTimer;
    delete  mPaperLogic;
}


cServer::cServer() :
    QTcpServer()
{
    mUpdateTimer = new  QTimer();
    mUpdateTimer->start( 1000/60 );

    mPacketTimer = new  QTimer();
    mPacketTimer->start( 1000/10 );

    connect( mUpdateTimer, &QTimer::timeout, this, &cServer::Update );
    connect( mPacketTimer, &QTimer::timeout, this, &cServer::NetworkTick );
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
cServer::SendGridToClient( QTcpSocket * iClient )
{
    QByteArray dataCompressed;
    QDataStream streamComp( &dataCompressed, QIODevice::WriteOnly );
    streamComp << *mPaperLogic;

    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );
    stream.setDevice( iClient );


    stream << quint8(0);
    stream << qCompress( dataCompressed, 1 );
}


void
cServer::SendSimpleUserPositionToClient( QTcpSocket * iClient, cUser* iUser, eType iType )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );
    stream.setDevice( iClient);

    stream << quint8(1);
    stream << iType << *iUser;
}


void
cServer::Update()
{
    mPaperLogic->Update();

    if( mQuit )
        emit quit();
}


void
cServer::NetworkTick()
{
    for( auto client : mClients )
        SendGridToClient( client );
}


void
cServer::GetData()
{
    int header;

    int index = -1;

    for( auto stream : mDataStream )
    {
        stream->startTransaction();
        *stream >> header;
        if( !stream->commitTransaction() ) // If packet isn't complete, this will restore data to initial position, so we can read again on next GetData
            continue;

        index = mDataStream.key( stream );
        break;
    }

    if( index == -1 )
        return;

    switch( header )
    {
        case 1: // Right
            mPaperLogic->mAllUsers[ index ]->setMovementVector( QPoint( -1, 0 ) );
            break;
        case 2: // Left
            mPaperLogic->mAllUsers[ index ]->setMovementVector( QPoint( 1, 0 ) );
            break;
        case 3: // Top
            mPaperLogic->mAllUsers[ index ]->setMovementVector( QPoint( 0, -1 ) );
            break;
        case 4: // Bottom
            mPaperLogic->mAllUsers[ index ]->setMovementVector( QPoint( 0, 1 ) );
            break;

        case 10 : // Respawn
            mPaperLogic->TryRespawningPlayer( mPaperLogic->mAllUsers[ index ] );
            break;

        default:
            break;
    }
}


void
cServer::NewClientConnected( )
{
    qDebug() << "New client connected";

    int R = rand() % 126;
    int G = rand() % 126;
    int B = rand() % 126;
    auto newUser = new cUser( mPaperLogic->mAllUsers.size(), QColor( R, G, B ) );
    mPaperLogic->AddUser( newUser );

    // Tell all users a new one came
    for( auto client : mClients )
        SendSimpleUserPositionToClient( client, newUser, kOtherUser );

    // Add new client
    auto it = mClients.insert( newUser->mIndex, nextPendingConnection() );
    auto newClient = it.value();
    connect( newClient, &QTcpSocket::readyRead, this, &cServer::GetData );

    auto dstream = new QDataStream( newClient );
    dstream->setVersion( QDataStream::Qt_5_10 );
    mDataStream.insert( newUser->mIndex, dstream );

    // Tell him his own position ( so client knows which player its controlling )
    SendSimpleUserPositionToClient( newClient, newUser, kSelfUser );

    // Tell new client about others
    for( auto client : mClients )
    {
        if( client == newClient )
            continue;

        cUser* user = mPaperLogic->mAllUsers[ mClients.key( client ) ];
        SendSimpleUserPositionToClient( newClient, user, kOtherUser );
    }

}



