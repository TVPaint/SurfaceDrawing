#pragma once

#include <QGraphicsView>
#include <QMap>


class cItemUser;
class cBasicTile;
class cUser;
class cPaperLogic;
class cItemGrid;
class  cBordersItem;

class cCanvas :
    public QGraphicsView
{
    Q_OBJECT


public:
    enum eUserType
    {
        kMyself,
        kOther
    };

public:
    ~cCanvas();
    cCanvas( cPaperLogic* iPaperLogic, QWidget* parent );


public:
    void  keyPressEvent( QKeyEvent* iEvent ) override;
    void  keyReleaseEvent( QKeyEvent* iEvent ) override;
    void  resizeEvent( QResizeEvent* event ) override;


public:
    void  AddUser( cUser* iUser, eUserType iUserType );

public:
    void  Update();

signals:
    void  directionChanged( quint64 iTick, int iDirection );
    void  compRequest( quint64 iTick, int iCompNumber );
    void  compStopRequest( quint64 iTick, int iCompNumber );
    void  respawnRequest();
    void  pingRequest();
    void  rollbackTest();
    void  askResync();

public:
    cItemUser*                  mMyself = 0;

    QMap< int, cItemUser* >     mAllUserItems;
    QVector< cBasicTile* >      mAllTiles;
    cPaperLogic*                mPaperLogic;
    cItemGrid*                  mGrid;
    cBordersItem*               mBordersItem;
};
