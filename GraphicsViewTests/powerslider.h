#pragma once

#include <QSlider>



class powerslider :
    public QSlider
{
    Q_OBJECT

public:
    powerslider(QWidget *parent);
    ~powerslider();
};
