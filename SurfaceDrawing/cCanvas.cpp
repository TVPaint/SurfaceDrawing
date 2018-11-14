#include "cCanvas.h"


#include "cUser.h"
#include "cItemUser.h"
#include "cBasicTile.h"
//#include "cItemGrid.h"
#include "cItemPlayfield.h"
#include "cPaperLogic.h"
#include "cBordersItem.h"
#include "cUIItemComp.h"


#include <QGraphicsScene>
#include <QKeyEvent>


cCanvas::~cCanvas()
{
    // when the scene deletes it'll delete items inside
}


cCanvas::cCanvas( cPaperLogic* iPaperLogic, QWidget* parent ) :
    QGraphicsView( parent ),
    mPaperLogic( iPaperLogic )
{
    int offset = 600;
    int gridSize = GRIDSIZE * CELLSIZE + offset*2;

    QGraphicsScene* scene = new QGraphicsScene();
    scene->setSceneRect( -offset, -offset, gridSize, gridSize );
    setScene( scene );

    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );


    //setStyleSheet( "background: #404050" ); // Lags the game !


    mItemPlayfied = new cItemPlayfield( 0 );
    scene->addItem( mItemPlayfied );

    //mGrid = new cItemGrid( 0 );
    //mGrid->mSize = size();
    //scene->addItem( mGrid );

    mBordersItem = new cBordersItem();
    scene->addItem( mBordersItem );

    for( int i = 0; i < GRIDSIZE * GRIDSIZE; ++i )
    {
        auto tile = new cBasicTile();
        tile->mColor = QColor( Qt::transparent );
        tile->setPos( QPoint(       (i%GRIDSIZE) * CELLSIZE,
                                    (i/GRIDSIZE) * CELLSIZE )
                    );
        mAllTiles.push_back( tile );
        scene->addItem( tile );
    }

    mPaperLogic->AddGridChangedCB(

        [ this ]( int x, int y, int newValue, cPaperLogic::eDataType iType )
        {
            int singleIndex = x + y * GRIDSIZE;
            auto tile = mAllTiles[ singleIndex ];
            tile->mHalf = false;

            tile->mGroundColor  = mPaperLogic->GetColorByIndex( mPaperLogic->mPaperGrid[x][y].mGround ).darker( 170 );
            tile->mTrailColor   = mPaperLogic->GetColorByIndex( mPaperLogic->mPaperGrid[x][y].mTrail ).lighter( 170 );

            if( iType == cPaperLogic::kPlayer )
            {
                if( newValue >= 0 )
                {
                    if( mPaperLogic->mPaperGrid[x][y].mGround < 0 )
                    {
                        tile->mHalf = true;
                        tile->mTrailColor   = mPaperLogic->GetColorByIndex( mPaperLogic->mPaperGrid[x][y].mPlayer ).lighter( 170 );

                        if( mAllUserItems[ newValue ]->mUser->mGUICurrentMovementVector.x() > 0 )
                            tile->mDirection = cBasicTile::kEast;
                        else if( mAllUserItems[ newValue ]->mUser->mGUICurrentMovementVector.x() < 0 )
                            tile->mDirection = cBasicTile::kWest;
                        else if( mAllUserItems[ newValue ]->mUser->mGUICurrentMovementVector.y() > 0 )
                            tile->mDirection = cBasicTile::kSouth;
                        else if( mAllUserItems[ newValue ]->mUser->mGUICurrentMovementVector.y() < 0 )
                            tile->mDirection = cBasicTile::kNorth;
                    }
                }
            }

            tile->update();
        }
    );
}


// ==================================================================================
// ==================================================================================
// ==================================================================================


void
cCanvas::keyPressEvent( QKeyEvent * iEvent )
{
    if( iEvent->key() == Qt::Key_Left )
    {
        mMyself->mUser->setMovementVector( QPoint( -1, 0 ) );
        emit directionChanged( mPaperLogic->mTick, 1 );
    }
    else if( iEvent->key() == Qt::Key_Right )
    {
        mMyself->mUser->setMovementVector( QPoint( 1, 0 ) );
        emit directionChanged( mPaperLogic->mTick, 2 );
    }
    else if( iEvent->key() == Qt::Key_Up )
    {
        mMyself->mUser->setMovementVector( QPoint( 0, -1 ) );
        emit directionChanged( mPaperLogic->mTick, 3 );
    }
    else if( iEvent->key() == Qt::Key_Down )
    {
        mMyself->mUser->setMovementVector( QPoint( 0, 1 ) );
        emit directionChanged( mPaperLogic->mTick, 4 );
    }
    else if( iEvent->key() == Qt::Key_Space )
    {
        mPaperLogic->SetUserAskedRespawn( mMyself->mUser );
        emit respawnRequest();
    }
    else if( iEvent->key() == Qt::Key_Ampersand )
    {
        mMyself->mUser->activateComp( 0 );
        emit compRequest( mPaperLogic->mTick, 0 );
    }
    else if( iEvent->key() == Qt::Key_P )
    {
        emit pingRequest();
    }
    else if( iEvent->key() == Qt::Key_R )
    {
        emit rollbackTest();
    }
    else if( iEvent->key() == Qt::Key_U )
    {
        emit askResync();
    }
}


void
cCanvas::keyReleaseEvent( QKeyEvent * iEvent )
{
    if( iEvent->key() == Qt::Key_Ampersand && !iEvent->isAutoRepeat() )
    {
        mMyself->mUser->deactivateComp( 0 );
        emit compStopRequest( mPaperLogic->mTick, 0 );
    }
}


void
cCanvas::resizeEvent( QResizeEvent* event )
{
    //mGrid->mSize = size() + QSize( CELLSIZE, CELLSIZE );
    //mGrid->update();
    QGraphicsView::resizeEvent( event );
}



// ==================================================================================
// ==================================================================================
// ==================================================================================


void
cCanvas::AddUser( cUser* iUser, eUserType iUserType )
{
    if( iUser->mIndex < 0 )
        return;

    auto userItem = new cItemUser( iUser );
    mAllUserItems.insert( iUser->mIndex, userItem );
    scene()->addItem( userItem );

    if( iUserType == kMyself )
    {
        mMyself = mAllUserItems[ iUser->mIndex ];
        auto uiRollback = new cUIItemComp( mMyself->mUser, 0 );
        uiRollback->SetImage( ":/SurfaceDrawing/Resources/Icone.png" );
        uiRollback->SetGrayImage( ":/SurfaceDrawing/Resources/IconeGray.png" );
        mUIItemsComps.push_back( uiRollback );
        scene()->addItem( uiRollback );
    }
}


void
cCanvas::Update()
{
    //QPointF firstPos = mapToScene( 0, 0 );
    //firstPos.setX( firstPos.x() - (int(firstPos.x()) %  CELLSIZE) );
    //firstPos.setY( firstPos.y() - (int(firstPos.y()) %  CELLSIZE) );
    //mGrid->setPos( firstPos );

    for( auto item : mAllUserItems )
        item->setPos( item->mUser->mGUIPosition );

    ensureVisible( mMyself, width()/2-CELLSIZE - 2, height()/2-CELLSIZE - 2 );

    QPointF uiPos = mapToScene( size().width()/2 - UICOMPSIZE/2, size().height() - UICOMPSIZE - 5 );

    for( auto uiItem : mUIItemsComps )
    {
        uiItem->setPos( uiPos );
        uiItem->Update();
    }
}








