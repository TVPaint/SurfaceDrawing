#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SurfaceDrawing.h"

class SurfaceDrawing : public QMainWindow
{
    Q_OBJECT

public:
    SurfaceDrawing(QWidget *parent = Q_NULLPTR);

private:
    Ui::SurfaceDrawingClass ui;
};
