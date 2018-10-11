#pragma once

#include <QGraphicsPixmapItem >

#include <filesystem>

class  cCustomGraphicsView;

class cEditableItem :
    public QGraphicsPixmapItem
{
public:
    cEditableItem( QGraphicsItem *parent = Q_NULLPTR );

public:
    void setFile( const QString& iFile );

protected:
    void        mousePressEvent( QGraphicsSceneMouseEvent* iEvent ) override;
    void        mouseMoveEvent( QGraphicsSceneMouseEvent* iEvent ) override;
    void        mouseReleaseEvent( QGraphicsSceneMouseEvent* iEvent ) override;

    void        contextMenuEvent( QGraphicsSceneContextMenuEvent* iEvent ) override;

private:
    std::filesystem::path  mFileToEdit;
};

