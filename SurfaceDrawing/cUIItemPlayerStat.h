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

public:


private:
    QRectF iconBoundingRect() const;
    QRectF textBoundingRect() const;

public:
    cPaperLogic*  mPaperLogic;
    cUser*        mUser;
};
