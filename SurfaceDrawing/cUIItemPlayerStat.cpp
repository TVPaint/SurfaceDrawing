#include "cUIItemPlayerStat.h"


#include "cPaperLogic.h"
#include "cUser.h"

#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QGraphicsColorizeEffect>



cUIItemPlayerStat::~cUIItemPlayerStat()
{
}


cUIItemPlayerStat::cUIItemPlayerStat( cPaperLogic*  iPaperLogic, cUser* iUser, QGraphicsItem* iParent ) :
    QGraphicsItem( iParent ),
    mPaperLogic( iPaperLogic ),
    mUser( iUser )
{
}


QRectF
cUIItemPlayerStat::boundingRect() const
{
    return   UI_ITEM_RECT;
}


void
cUIItemPlayerStat::paint( QPainter* iPainter, const QStyleOptionGraphicsItem* iOptions, QWidget* iWidget )
{
    QColor color = mUser->mColor;

    iPainter->setBrush( color );
    iPainter->setPen( Qt::black );

    QString  text = QString::number( mPaperLogic->ComputeLandUsage( mUser ) * 100, 'f', 2 );

    iPainter->drawRect( iconBoundingRect() );
    iPainter->drawText( textBoundingRect(), text );
}


QRectF
cUIItemPlayerStat::iconBoundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), QSize( UI_ICON_SIZE, UI_ICON_SIZE ) );
}


QRectF
cUIItemPlayerStat::textBoundingRect() const
{
    return   QRect( QPoint( UI_ICON_SIZE + UI_TEXT_OFFSET, 0 ), QPoint( UI_TEXT_WIDTH, UI_ITEM_HEIGHT ) );
}

