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

    connect( mClientSocket, &cClient::newUserArrived, this, &SurfaceDrawing::NewUserArrived );
    connect( mClientSocket, &cClient::myUserAssigned, this, &SurfaceDrawing::MyUserAssigned );
}


void
SurfaceDrawing::Update()
{
    mPaperLogic->Update();
    mCanvas->Update();
}


void
SurfaceDrawing::Start()
{
    mTimer = new  QTimer();
    mTimer->start( 1000/60 );

    connect( mTimer, &QTimer::timeout, this, &SurfaceDrawing::Update );
}


void
SurfaceDrawing::MyUserAssigned( cUser * iUser )
{
    mCanvas->AddUser( iUser, cCanvas::kMyself );
    mPaperLogic->AddUser( iUser );
    Start();
}


void
SurfaceDrawing::PaperLogicArrived( cPaperLogic & iPaper )
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

    mPaperLogic->CopyFromPaper( iPaper );
}


void
SurfaceDrawing::NewUserArrived( cUser* iUser )
{
    mCanvas->AddUser( iUser, cCanvas::kOther );
    mPaperLogic->AddUser( iUser );
}



