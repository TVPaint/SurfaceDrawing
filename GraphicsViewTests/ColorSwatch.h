#pragma once

#include <QLabel>


class ColorSwatch :
    public QLabel
{
    Q_OBJECT

public:
    virtual ~ColorSwatch();
    ColorSwatch( QWidget* iParent = nullptr );

public:
    void  mouseReleaseEvent( QMouseEvent* iEvent ) override;

public:
    void  SetColor( const QColor& iColor );

signals:
    void  swatchClicked();
};

