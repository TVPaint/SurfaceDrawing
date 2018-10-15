#pragma once

#include <QWidget>

class colorPicker :
    public QWidget
{
    Q_OBJECT

public:
    ~colorPicker();
    colorPicker( QWidget *parent);

public:
    void  paintEvent( QPaintEvent* event ) override;
};
