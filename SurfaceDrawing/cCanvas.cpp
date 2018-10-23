#include "cCanvas.h"


#include "cUser.h"
#include "cItemUser.h"
#include "cBasicTile.h"
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

        [ this ]( int x, int y, int newValue )
        {
            if( newValue < 10 )
                return;

            int singleIndex = x + y*GRIDSIZE;
            QColor color = cPaperLogic::GetColorByIndex( 0 );

            if( newValue == 11 )
                color = color.lighter( 170 );
            else if( newValue > 20 && newValue < 30 )
                color = color.darker( 170 );

            mAllTiles[ singleIndex ]->mColor = color;
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
}


// ==================================================================================
// ==================================================================================
// ==================================================================================


void
cCanvas::AddUser( cUser* iUser )
{
    mAllUserItems.push_back( new cItemUser( iUser ) );
    scene()->addItem( mAllUserItems.back() );

    //QPoint userPosition = iUser->mPosition;
    //int singleIndex = userPosition.x() + userPosition.y() * GRIDSIZE;

    //// Middle row
    //for( int i = singleIndex - 1; i < singleIndex+1 ; ++i )
    //    mAllTiles[ i ]->mColor = mAllUserItems[ 0 ]->mUser->mColor.darker( 170 );

    //// Top Row
    //singleIndex -= GRIDSIZE;
    //for( int i = singleIndex - 1; i < singleIndex+1 ; ++i )
    //    mAllTiles[ i ]->mColor = mAllUserItems[ 0 ]->mUser->mColor.darker( 170 );

    //// BottomRow
    //singleIndex += 2*GRIDSIZE;
    //for( int i = singleIndex - 1; i < singleIndex+1 ; ++i )
    //    mAllTiles[ i ]->mColor = mAllUserItems[ 0 ]->mUser->mColor.darker( 170 );


}


void
cCanvas::Update()
{
    int counter = 0;
    for( auto item : mAllUserItems )
    {
        item->setPos( mAllUserItems[ counter ]->mUser->mGUIPosition );
        ++counter;
    }

    ensureVisible( mAllUserItems[ 0 ], width()/2-CELLSIZE, height()/2-CELLSIZE  );
}
