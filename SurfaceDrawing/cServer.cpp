#include "cServer.h"

#include "cUser.h"

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
cServer::SendGridToClient( QTcpSocket * iClient )
{
    QByteArray data;

        QDataStream stream( &data, QIODevice::WriteOnly );
        stream.setVersion( QDataStream::Qt_5_10 );
        stream.setDevice( iClient );

        stream << quint8(0);
        stream << *mPaperLogic;
}


void
cServer::SendSimpleUserPositionToClient( QTcpSocket * iClient, cUser* iUser, eType iType )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );
    stream.setDevice( iClient);

    stream << quint8(1);
    if( iType == kOtherUser )
        stream << QString( "other-" + QString::number( iUser->mIndex ) + "-" + QString::number( iUser->mPosition.x() ) + "," + QString::number( iUser->mPosition.y() ) );
    else if( iType == kSelfUser )
        stream << QString( "self-" + QString::number( iUser->mIndex ) + "-" + QString::number( iUser->mPosition.x() ) + "," + QString::number( iUser->mPosition.y() ) );
}


void
cServer::Update()
{
    mPaperLogic->Update();
    for( auto client : mClients )
        SendGridToClient( client );

    if( mQuit )
        emit quit();
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
        case 1:
            mPaperLogic->mAllUsers[ index ]->mAskDirectionChange = true;
            mPaperLogic->mAllUsers[ index ]->mGUIMovementVector = QPoint( -1, 0 );
            break;
        case 2:
            mPaperLogic->mAllUsers[ index ]->mAskDirectionChange = true;
            mPaperLogic->mAllUsers[ index ]->mGUIMovementVector = QPoint( 1, 0 );
            break;
        case 3:
            mPaperLogic->mAllUsers[ index ]->mAskDirectionChange = true;
            mPaperLogic->mAllUsers[ index ]->mGUIMovementVector = QPoint( 0, -1 );
            break;
        case 4:
            mPaperLogic->mAllUsers[ index ]->mAskDirectionChange = true;
            mPaperLogic->mAllUsers[ index ]->mGUIMovementVector = QPoint( 0, 1 );
            break;

        default:
            break;
    }
}


void
cServer::NewClientConnected( )
{
    qDebug() << "New client connected";

    auto newUser = new cUser( mPaperLogic->mAllUsers.size() );
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



