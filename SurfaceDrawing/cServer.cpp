#include "cServer.h"

#include "cUser.h"
#include "cPaperLogic.h"

#include <QHostAddress>
#include <ctime>


#define CLOCKTIME 108000000 // 30 min

cServer::~cServer()
{
    delete  mUpdateTimer;
    delete  mPaperLogic;

    mDEBUGFile->close();
    delete  mDEBUGFile;
    delete  mDEBUGStream;
}


cServer::cServer() :
    QTcpServer()
{
    mUpdateTimer = new  QTimer();
    mUpdateTimer->start( SPEED/2 );

    mApplicationClock = new QTimer();
    mApplicationClock->start( CLOCKTIME );

    mPreviousTime = qint64( mApplicationClock->remainingTimeAsDuration().count() );

    connect( mUpdateTimer, &QTimer::timeout, this, &cServer::Update );
    connect( mApplicationClock, &QTimer::timeout, this, &cServer::SendClockToAllClients );

    connect( this, &QTcpServer::newConnection, this, &cServer::NewClientConnected );

    mPaperLogic = new cPaperLogic();
    mPaperLogic->Init();

    mDEBUGFile = new QFile( "./DEBUGLOGS/ServerLogs.txt" );
    if( !mDEBUGFile->open( QIODevice::WriteOnly ) )
        qDebug() << "Can't open file";

    mDEBUGStream = new QTextStream( mDEBUGFile );
}


void
cServer::Run()
{
    if( !listen( QHostAddress::Any, 55666 ) )
        qDebug() << "Error";

}


void
cServer::SendGridToAllClient()
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );

    stream << qint64( mApplicationClock->remainingTimeAsDuration().count() );
    stream << quint8(kGrid);
    stream << *mPaperLogic;

    _LOG( "Sending grid to all clients" );

    for( auto client : mClients )
        client->write( data );
}


void
cServer::SendClockToAllClients()
{
    _LOG( "Sending clock signal to all clients" );
    for( auto client : mClients )
    {
        QByteArray data;
        QDataStream stream( &data, QIODevice::WriteOnly );
        stream.setVersion( QDataStream::Qt_5_10 );
        stream.setDevice( client );

        stream << qint64( mApplicationClock->remainingTimeAsDuration().count() );
        stream << quint8(kClock);
    }

    _LOG( "DONE sending clock signal to all clients" );
}


void
cServer::SendSimpleUserPositionToClient( QTcpSocket * iClient, cUser* iUser, eType iType )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );
    stream.setDevice( iClient);


    QString type = "kSelfAssign";
    if( iType == kOtherUser )
        type = "kOtherUser";

    _LOG( "Sending " + type + " information to client : " + QString::number( mClients.key( iClient ) ) );

    stream << qint64( mApplicationClock->remainingTimeAsDuration().count() );
    stream << quint8(kSimple);
    stream << iType;
    stream << *iUser;
    stream << mPaperLogic->mTick;

    _LOG( "DONE sending information" );

}


void
cServer::SendUserActionToClient( QTcpSocket * iClient, cUser * iUser, int iAction )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );
    stream.setDevice( iClient );

    _LOG( "Sending action to user : " + QString::number( mClients.key( iClient ) ) );

    stream << qint64( mApplicationClock->remainingTimeAsDuration().count() );
    stream << quint8(kAction);
    stream << iAction;
    stream << *iUser;
    stream << mPaperLogic->mTick;

    _LOG( "DONE sending action" );
}


void
cServer::SendUserDisconnectedToAllClients( int iIndex )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );

    stream << qint64( mApplicationClock->remainingTimeAsDuration().count() );
    stream << quint8(kDisc);
    stream << iIndex;


    _LOG( "Sending disconnected msg to all clients" );

    for( auto client : mClients )
        client->write( data );

    _LOG( "DONE sending disconnect info" );
}


void
cServer::SendPongToClient( QTcpSocket* iClient )
{
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_10 );
    stream.setDevice( iClient );

    _LOG( "Sending pong" );

    stream << qint64( mApplicationClock->remainingTimeAsDuration().count() );
    stream << quint8(kPong);

    _LOG( "DONE sending pong" );
}


void
cServer::BuildPacket( QByteArray * oData, QDataStream * oStream, int iType )
{
    // possible ?
}


bool
cServer::ReadUserAction( int iClientIndex, int iAction )
{
    QDataStream* stream = mDataStream[ iClientIndex ];
    cUser* user = mPaperLogic->mAllUsers[ iClientIndex ];

    quint64 tick;

    stream->startTransaction();
    *stream >> tick;
    if( !stream->commitTransaction() ) // If packet isn't complete, this will restore data to initial position, so we can read again on next GetData
        return  false;

    _LOG( "User : " + QString::number( iClientIndex ) + " did an action " + QString::number( iAction ) + " -- Requested at tick : " + QString::number( tick ) );

    QPoint  newVector;
    switch( iAction )
    {
        case 1: // Left
            newVector = ( QPoint( -1, 0 ) );
            break;
        case 2: // Right
            newVector = ( QPoint( 1, 0 ) );
            break;
        case 3: // Top
            newVector = ( QPoint( 0, -1 ) );
            break;
        case 4: // Bottom
            newVector = ( QPoint( 0, 1 ) );
            break;

        default:
            break;
    }

    mPaperLogic->GoToTick( tick ); // Lag Compensation
    user->setMovementVector( newVector );
    mPaperLogic->GoToTick( mPaperLogic->mTick );

    // Information to other clients
    for( auto client : mClients )
    {
        if( iClientIndex == mClients.key( client ) ) // Don't send to the guy that initially sent us the action
            continue;

        SendUserActionToClient( client, mPaperLogic->mAllUsers[ iClientIndex ], iAction );
    }

    return  true;
}


void
cServer::ClientDisconnected()
{
    for( auto client : mClients )
    {
        if( client->state() == QAbstractSocket::UnconnectedState )
        {
            auto clientKey = mClients.key( client );
            _LOG( "Client " + QString::number( clientKey ) + " disconnected." );

            // Remove from containers
            mDataStream.erase( mDataStream.find( clientKey ) );
            mClients.erase( mClients.find( clientKey ) );
            mPaperLogic->RemoveUser( mPaperLogic->mAllUsers[ clientKey ] );

            client->deleteLater();

            SendUserDisconnectedToAllClients( clientKey );
            break;
        }
    }

    SendGridToAllClient();
}


void
cServer::Update()
{
    mPaperLogic->Update( qint64( mApplicationClock->remainingTimeAsDuration().count() ) );

    if( mQuit )
        emit quit();
}


void
cServer::NetworkTick()
{
    SendGridToAllClient();
}


void
cServer::GetData()
{
    int header;
    int index = -1;


    while( _CheckForData() )
    {
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
            // Movement
            case 1: // Left
            case 2: // Right
            case 3: // Top
            case 4: // Bottom
                if( !ReadUserAction( index, header ) ) // false == packet incomplete, so return and wait the next one
                    return;
                break;

            case 10 : // Respawn
                mPaperLogic->TryRespawningPlayer( mPaperLogic->mAllUsers[ index ] );
                break;

            case 99 : // Ping
                _LOG( "User : " + QString::number( index ) + " did a ping request" );
                SendPongToClient( mClients[ index ] );
                break;

            case 666 : // ClientIsReady
                _LOG( "User : " + QString::number( index ) + " is ready to communicate" );
                // Tell him his own position ( so client knows which player its controlling )
                SendSimpleUserPositionToClient( mClients[ index ], mPaperLogic->mAllUsers[ index ], kSelfUser );

                // Tell new client about others
                for( auto client : mClients )
                {
                    if( client == mClients[ index ] )
                        continue;

                    cUser* user = mPaperLogic->mAllUsers[ mClients.key( client ) ];
                    SendSimpleUserPositionToClient( mClients[ index ], user, kOtherUser );
                }

                SendGridToAllClient();
                break;

            default:
                break;
        }

        if( header == 10 ) // Respawn
        {
            for( auto client : mClients )
            {
                if( index == mClients.key( client ) ) // Don't send to the guy that initially sent us the action
                    continue;

                SendUserActionToClient( client, mPaperLogic->mAllUsers[ index ], header );
            }
        }
    }
}


void
cServer::_LOG( const QString & iText )
{
    qDebug() << mApplicationClock->remainingTimeAsDuration().count() << " : " << iText;
    *mDEBUGStream << mApplicationClock->remainingTimeAsDuration().count() << " : " << iText << "\r" << endl;
}


bool
cServer::_CheckForData()
{
    for( auto client : mClients )
    {
        if( client->bytesAvailable() > 0 )
            return  true;
    }
    return  false;
}


void
cServer::NewClientConnected( )
{
    _LOG( "New client connected" );

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
    connect( newClient, &QTcpSocket::disconnected, this, &cServer::ClientDisconnected );

    auto dstream = new QDataStream( newClient );
    dstream->setVersion( QDataStream::Qt_5_10 );
    mDataStream.insert( newUser->mIndex, dstream );

    // Clock sync
    SendClockToAllClients();
}



