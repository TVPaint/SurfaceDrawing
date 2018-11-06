#include "SurfaceDrawing.h"


#include "TickerTimer.h"


#include <QDebug>

SurfaceDrawing::~SurfaceDrawing()
{
    delete  mPaperLogic;
    delete  mTimer;
}


SurfaceDrawing::SurfaceDrawing(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi( this );
    Init();
}


void
SurfaceDrawing::Init()
{
    mPaperLogic = new cPaperLogic();
    mPaperLogic->Init();


    mCanvas = new cCanvas( mPaperLogic, this );
    mMainLayout = new QVBoxLayout();
    mMainLayout->addWidget( mCanvas );
    ui.centralWidget->setLayout( mMainLayout );

    mClientSocket = new cClient();
    mClientSocket->AskConnection();

    connect( mCanvas, &cCanvas::directionChanged, this, &SurfaceDrawing::DirectionChanged );
    connect( mCanvas, &cCanvas::respawnRequest, this, &SurfaceDrawing::RespawnRequest );
    connect( mCanvas, &cCanvas::pingRequest, this, &SurfaceDrawing::PingRequest );


    connect( mClientSocket, &cClient::newUserArrived, this, &SurfaceDrawing::NewUserArrived );
    connect( mClientSocket, &cClient::myUserAssigned, this, &SurfaceDrawing::MyUserAssigned );
    connect( mClientSocket, &cClient::paperLogicArrived, this, &SurfaceDrawing::PaperLogicArrived );
    connect( mClientSocket, &cClient::userChangedDirection, this, &SurfaceDrawing::UserDirectionChanged );
    connect( mClientSocket, &cClient::userRequestedRespawn, this, &SurfaceDrawing::UserRequestedRespawn );
    connect( mClientSocket, &cClient::userDisconnected, this, &SurfaceDrawing::UserDisconnected );
}


void
SurfaceDrawing::Update()
{
    mPaperLogic->Update( mClientSocket->mApplicationClock->remainingTimeAsDuration().count() );
    mCanvas->Update();
}


void
SurfaceDrawing::Start()
{
    mTimer = new  QTimer();
    mTimer->start( SPEED/2 );

    connect( mTimer, &QTimer::timeout, this, &SurfaceDrawing::Update );
}


void
SurfaceDrawing::RespawnRequest()
{
    mClientSocket->SendRespawnRequest();
}


void
SurfaceDrawing::DirectionChanged( quint64 iTick, int iDirection )
{
    mClientSocket->SendNewDirection( iTick, iDirection );
    qDebug() << mPaperLogic->mTick;
}


void
SurfaceDrawing::PingRequest()
{
    mClientSocket->SendPing();
}


void
SurfaceDrawing::PaperLogicArrived( cPaperLogic & iPaper, int  iLatencyInMs )
{
    // As we send newUser and otherUsers info, this should never be out of sync
    Q_ASSERT( iPaper.mAllUsers.size() == mPaperLogic->mAllUsers.size() );

    //if( iPaper.mAllUsers.size() != mPaperLogic->mAllUsers.size() )
    //{
    //    for( auto user : iPaper.mAllUsers )
    //    {
    //        int  index = iPaper.mAllUsers.key( user );
    //        if( mPaperLogic->mAllUsers.find( index ) != mPaperLogic->mAllUsers.end() )
    //            continue;

    //        auto newUser = new cUser( index );
    //        mCanvas->AddUser( newUser );
    //        mPaperLogic->AddUser( newUser );
    //    }
    //}

    qDebug() << "PAPER LATENCY = " << iLatencyInMs;

    mPaperLogic->CopyFromPaper( iPaper, 0 );

    quint16 missingUpdates = iLatencyInMs / SPEED;

    qDebug() << "OFF BY -------------> " + QString::number( missingUpdates );
}


void
SurfaceDrawing::NewUserArrived( cUser* iUser )
{
    mCanvas->AddUser( iUser, cCanvas::kOther );
    mPaperLogic->AddUser( iUser );
}


void
SurfaceDrawing::MyUserAssigned( cUser * iUser )
{
    mCanvas->AddUser( iUser, cCanvas::kMyself );
    mPaperLogic->AddUser( iUser );
    Start();
}


void
SurfaceDrawing::UserDirectionChanged( cUser * iUser, quint64 iTick )
{
    mPaperLogic->GoToTick( iTick );
    mPaperLogic->mAllUsers[ iUser->mIndex ]->setMovementVector( iUser->mGUIMovementVector );
    mPaperLogic->GoToTick( mPaperLogic->mTick );
}


void
SurfaceDrawing::UserRequestedRespawn( cUser * iUser )
{
    mPaperLogic->TryRespawningPlayer( mPaperLogic->mAllUsers[ iUser->mIndex ] );
}


void
SurfaceDrawing::UserDisconnected( int iIndex )
{
    mPaperLogic->RemoveUser( mPaperLogic->mAllUsers[ iIndex ] );
}



