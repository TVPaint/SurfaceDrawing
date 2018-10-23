#pragma once

#include <QGraphicsView>


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



public:
    void  AddUser( cUser* iUser );

public:
    void  Update();

public:
    QVector< cItemUser* >       mAllUserItems;
    QVector< cBasicTile* >      mAllTiles;
    cPaperLogic*                mPaperLogic;
};
