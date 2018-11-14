#pragma once

#include <QGraphicsItem>
#include <QGraphicsColorizeEffect>

class  cPaperLogic;
class  cUser;

class cUIItemPlayerStat :
    public QGraphicsItem
{
public:
    ~cUIItemPlayerStat();
    cUIItemPlayerStat( cPaperLogic*  iPaperLogic, cUser* iUser, QGraphicsItem* iParent = 0);

public:
    QRectF boundingRect() const override;
    void   paint( QPainter* iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget* iWidget = 0 )  override;

private:
    QRectF iconBoundingRect() const;
    QRectF textBoundingRect() const;

public:
    static constexpr  int  UI_ICON_SIZE = 16; 
    static constexpr  int  UI_TEXT_OFFSET = 3; 
    static constexpr  int  UI_TEXT_WIDTH = 61; 
    static constexpr  int  UI_ITEM_WIDTH = UI_ICON_SIZE + UI_TEXT_OFFSET + UI_TEXT_WIDTH;
    static constexpr  int  UI_ITEM_HEIGHT = UI_ICON_SIZE;
    static constexpr  QRectF  UI_ITEM_RECT = QRectF( QPoint( 0, 0 ), QSize( UI_ITEM_WIDTH, UI_ITEM_HEIGHT ) );

public:
    cPaperLogic*  mPaperLogic;
    cUser*        mUser;
};
