#include "cCanvas.h"


#include "cUser.h"
#include "cItemUser.h"
#include "cBasicTile.h"
#include "cItemGrid.h"
#include "cPaperLogic.h"


#include <QGraphicsScene>
#include <QKeyEvent>


cCanvas::~cCanvas()
{
}


cCanvas::cCanvas( cPaperLogic* iPaperLogic, QWidget* parent ) :
    QGraphicsView( parent ),
    mPaperLogic( iPaperLogic )
{
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setSceneRect( 0, 0, GRIDSIZE * CELLSIZE, GRIDSIZE * CELLSIZE );
    setScene( scene );

    mGrid = new cItemGrid( 0 );
    mGrid->mSize = size();
    scene->addItem( mGrid );

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

            QColor color = Qt::transparent;
            if( mPaperLogic->mPaperGrid[x][y].mGround >= 0 )
                color = cPaperLogic::GetColorByIndex( mPaperLogic->mPaperGrid[x][y].mGround ).darker( 170 );

            if( mPaperLogic->mPaperGrid[x][y].mTrail >= 0 )
                color = cPaperLogic::GetColorByIndex( mPaperLogic->mPaperGrid[x][y].mTrail ).lighter( 170 );

            if( iType == cPaperLogic::kPlayer )
            {
                if( newValue >= 0 )
                {
                    if( mPaperLogic->mPaperGrid[x][y].mGround < 0 )
                    {
                        color = cPaperLogic::GetColorByIndex( mPaperLogic->mPaperGrid[x][y].mPlayer ).lighter( 170 );
                        tile->mHalf = true;
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

            tile->mColor = color;
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
        mAllUserItems[ 0 ]->mUser->setMovementVector( QPoint( -1, 0 ) );
    }
    else if( iEvent->key() == Qt::Key_Right )
    {
        mAllUserItems[ 0 ]->mUser->setMovementVector( QPoint( 1, 0 ) );
    }
    else if( iEvent->key() == Qt::Key_Up )
    {
        mAllUserItems[ 0 ]->mUser->setMovementVector( QPoint( 0, -1 ) );
    }
    else if( iEvent->key() == Qt::Key_Down )
    {
        mAllUserItems[ 0 ]->mUser->setMovementVector( QPoint( 0, 1 ) );
    }



    if( iEvent->key() == Qt::Key_Q )
    {
        mAllUserItems[ 1 ]->mUser->setMovementVector( QPoint( -1, 0 ) );
    }
    else if( iEvent->key() == Qt::Key_D )
    {
        mAllUserItems[ 1 ]->mUser->setMovementVector( QPoint( 1, 0 ) );
    }
    else if( iEvent->key() == Qt::Key_Z )
    {
        mAllUserItems[ 1 ]->mUser->setMovementVector( QPoint( 0, -1 ) );
    }
    else if( iEvent->key() == Qt::Key_S )
    {
        mAllUserItems[ 1 ]->mUser->setMovementVector( QPoint( 0, 1 ) );
    }
}


void
cCanvas::resizeEvent( QResizeEvent* event )
{
    mGrid->mSize = size() + QSize( CELLSIZE, CELLSIZE );
    mGrid->update();
}



// ==================================================================================
// ==================================================================================
// ==================================================================================


void
cCanvas::AddUser( cUser* iUser )
{
    auto userItem = new cItemUser( iUser );
    mAllUserItems.insert( iUser->mIndex, userItem );
    scene()->addItem( userItem );
}


void
cCanvas::Update()
{
    QPointF firstPos = mapToScene( 0, 0 );
    firstPos.setX( firstPos.x() - (int(firstPos.x()) %  CELLSIZE) );
    firstPos.setY( firstPos.y() - (int(firstPos.y()) %  CELLSIZE) );
    mGrid->setPos( firstPos );

    for( auto item : mAllUserItems )
        item->setPos( item->mUser->mGUIPosition );

    ensureVisible( mAllUserItems[ 0 ], width()/2-CELLSIZE - 2, height()/2-CELLSIZE - 2 );
}
