#include "cItemUser.h"


#include <QPainter>


cItemUser::~cItemUser()
{
}


cItemUser::cItemUser( cUser* iUser, QGraphicsItem * parent ) :
    QGraphicsItem( parent ),
    mUser( iUser )
{
}


QRectF
cItemUser::boundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), mUser->mGUISize );
}


void
cItemUser::paint( QPainter * iPainter, const QStyleOptionGraphicsItem * iOptions, QWidget * iWidget )
{
    if( mUser->mIsDead )
        return;

    auto size = mUser->mGUISize;
    QRadialGradient grad( mUser->mGUISize/2, mUser->mGUISize.x(), mUser->mGUISize/2 );
    grad.setColorAt( 0, mUser->mColor );
    grad.setColorAt( 1, mUser->mColor.darker( 120 ) );

    iPainter->setBrush( grad );
    iPainter->setPen( QPen( Qt::black ) );
    iPainter->drawRect( boundingRect() );
    return;

    //auto size = mUser->mGUISize;
    //QLinearGradient grad( QPoint( 0, 0 ), mUser->mGUISize );
    //grad.setColorAt( 0, mUser->mColor );
    //grad.setColorAt( 1, mUser->mColor.lighter( 200 ) );

    //iPainter->setBrush( grad );
    //iPainter->setPen( QPen( Qt::black ) );
    //iPainter->drawRect( boundingRect() );

}

