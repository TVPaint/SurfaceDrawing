#pragma once


#include <QAbstractListModel >
#include <QColor>
#include <QBrush>
#include <QPainter>
#include <QPen>

#include <filesystem>


class cToolModel :
    public QAbstractListModel
{
public:
    virtual ~cToolModel();
    cToolModel( QObject* iParent = Q_NULLPTR );

public:
    int             rowCount( const QModelIndex& iIndex ) const override;
    QVariant        data( const QModelIndex& iIndex, int iRole = Qt::DisplayRole ) const override;
    QVariant        headerData( int iSection, Qt::Orientation iOrientation, int iRole = Qt::DisplayRole ) const override;

    bool            setData( const QModelIndex& iIndex, const QVariant& iValue, int iRole = Qt::EditRole ) override;
    Qt::ItemFlags   flags( const QModelIndex& iIndex ) const override;


    QColor          getColor() const;
    void            setColor( const QColor& iColor );

    QPainter*       getNewPainter( QPaintDevice* iPaintDevice );
    QPen*           getPen();
    QBrush*         getBrush();

private:
    // Tools
    QPen*           mPen;
    QBrush*         mBrush;
    QColor          mAPen;
    int             mToolSize;
    bool            mAntiAliasingOn;
};

