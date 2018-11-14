#include "cUIItemComp.h"


#include "cPaperLogic.h"


#include <QBrush>
#include <QPainter>
#include <QPen>


cUIItemComp::~cUIItemComp()
{
}


cUIItemComp::cUIItemComp( cUser* iUser, int iCompIndex, QGraphicsItem* iParent ) :
    QGraphicsItem( iParent ),
    mUser( iUser ),
    mCompIndex( iCompIndex )
{
}


QRectF
cUIItemComp::boundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), QSize( UICOMPSIZE, UICOMPSIZE ) );
}


void
cUIItemComp::paint( QPainter * iPainter, const QStyleOptionGraphicsItem * iOptions, QWidget * iWidget )
{
    QRectF bbox = boundingRect();

    QColor color = QColor( 50, 150, 50, 180 );

    if( mActive )
        color = QColor( 150, 50, 50, 180 );
    else if( mCoolingdown )
        color = QColor( 50, 50, 50, 180 );

    iPainter->setBrush( color );
    iPainter->setPen( Qt::black );

    iPainter->drawRect( bbox );
}


void
cUIItemComp::Update()
{
    mActive        = mUser->mComps[ mCompIndex ].mActive;
    mCoolingdown   = mUser->mComps[ mCompIndex ].mCooldown > 0;
}


