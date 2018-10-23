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
    iPainter->setBrush( QBrush( mUser->mColor ) );
    iPainter->setPen( QPen( mUser->mColor ) );
    iPainter->drawRect( boundingRect() );
}

