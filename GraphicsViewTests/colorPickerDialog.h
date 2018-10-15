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

private:
    colorPicker* _colorPicker;
    QVBoxLayout* _mainLayout;
    QSlider*     _slider;

    cToolModel*  _ToolModel;
};

