#include "SurfaceDrawing.h"


#include "TickerTimer.h"


#include <QDebug>

SurfaceDrawing::~SurfaceDrawing()
{
    delete  mPaperLogic;
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
    sgTickerTimer->start( 1000/60 ); // 60FPmS ticks

    mPaperLogic = new cPaperLogic();
    mPaperLogic->Init();


    mCanvas = new cCanvas( mPaperLogic, this );
    mMainLayout = new QVBoxLayout();
    mMainLayout->addWidget( mCanvas );
    ui.centralWidget->setLayout( mMainLayout );


    connect( sgTickerTimer, &QTimer::timeout, this, &SurfaceDrawing::Update );

    mAllUsers.push_back( new cUser( cPaperLogic::GetColorByIndex( 0 ) ) );

    mPaperLogic->AddUser( mAllUsers.back() );
    mCanvas->AddUser( mAllUsers.back() );
}


void
SurfaceDrawing::Update()
{
    mPaperLogic->Update();
    mCanvas->Update();
}
