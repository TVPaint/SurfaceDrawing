#pragma once

#include <QGraphicsView>
#include <QMap>


class cItemUser;
class cBasicTile;
class cUser;
class cPaperLogic;

class cCanvas :
    public QGraphicsView
{
    Q_OBJECT

public:
    ~cCanvas();
    cCanvas( cPaperLogic* iPaperLogic, QWidget* parent );


public:
    void  keyPressEvent( QKeyEvent* iEvent ) override;
    void  paintEvent( QPaintEvent* iEvent ) override;



public:
    void  AddUser( cUser* iUser );

public:
    void  Update();

public:
    QMap< int, cItemUser* >       mAllUserItems;
    QVector< cBasicTile* >      mAllTiles;
    cPaperLogic*                mPaperLogic;
};
