#pragma once

#include "colorPicker.h"

#include "ToolModel.h"

#include  <QWidget>
#include  <QVBoxLayout>
#include  <QSlider>


class colorPickerDialog :
    public  QWidget
{

public:
    ~colorPickerDialog();
    colorPickerDialog( cToolModel* model, QWidget* iParent = nullptr );

    void  openAtPosition( QPointF iPos );

    QSize  size() const;

    void  sliderChanged( double iValue );
    void  slider2Changed( double iValue );
    void  colorPicked( const QColor& color );

private:
    colorPicker* _colorPicker;
    QVBoxLayout* _mainLayout;

    QHBoxLayout* _horlayout;

    QWidget*        _container;

    QSlider*     _slider;
    QSlider*     _slider2;

    cToolModel*  _ToolModel;
};

