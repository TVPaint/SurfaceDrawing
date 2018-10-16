#pragma once

#include <QWidget>
#include <QPixmap>
#include <QTimer>

class colorPicker :
    public QWidget
{
    Q_OBJECT

public:
    ~colorPicker();
    colorPicker( QWidget *parent );

public:
    void  paintEvent( QPaintEvent* event ) override;

    void  mousePressEvent( QMouseEvent* event ) override;
    void  mouseMoveEvent( QMouseEvent* event ) override;
    void  mouseReleaseEvent( QMouseEvent* event ) override;

    void  resizeEvent( QResizeEvent* event ) override;


    void drawWheel();
    void drawHUDCircle();
    void setLight( float light );
    float getLight() const;

    void setColor( const QColor& color );

    bool closeOnRelease() const;

signals:
    void  colorPicked( const QColor& color );

private:
    QPixmap*    _pixmap;
    QImage*     _image;

    QPixmap*    _hudCircle;
    QPoint      _hudPos;
    float       _light;

    QColor      _selColor;

    QTimer      _timer;
    bool        _closeOnRelease;
};
