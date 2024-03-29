/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include <qpainter.h>
#include <qpen.h>
#include "qwt_painter.h"
#include "qwt_scale_map.h"
#include "qwt_scale_div.h"
#include "qwt_plot_grid.h"

class QwtPlotGrid::PrivateData
{
public:
    PrivateData():
        xEnabled(true),
        yEnabled(true),
        xMinEnabled(false),
        yMinEnabled(false)
    {
    }

    bool xEnabled;
    bool yEnabled;
    bool xMinEnabled;
    bool yMinEnabled;

    QwtScaleDiv sdx;
    QwtScaleDiv sdy;

    QPen majPen;
    QPen minPen;
};

//! Enables major grid, disables minor grid
QwtPlotGrid::QwtPlotGrid()
{
    d_data = new PrivateData;
    setZ(10.0);
}

//! dtor
QwtPlotGrid::~QwtPlotGrid()
{
    delete d_data;
}

int QwtPlotGrid::rtti() const
{
    return QwtPlotItem::Rtti_PlotGrid;
}

/*!
  \brief Enable or disable vertical gridlines
  \param tf Enable (true) or disable

  \sa Minor gridlines can be enabled or disabled with
      QwtPlotGrid::enableXMin()
*/
void QwtPlotGrid::enableX(bool tf)
{
    if ( d_data->xEnabled != tf )
    {
        d_data->xEnabled = tf;
        gridChanged();
    }
}

/*!
  \brief Enable or disable horizontal gridlines
  \param tf Enable (true) or disable
  \sa Minor gridlines can be enabled or disabled with QwtPlotGrid::enableYMin()
*/
void QwtPlotGrid::enableY(bool tf)
{
    if ( d_data->yEnabled != tf )
    {
        d_data->yEnabled = tf;  
        gridChanged();
    }
}

/*!
  \brief Enable or disable  minor vertical gridlines.
  \param tf Enable (true) or disable
  \sa QwtPlotGrid::enableX()
*/
void QwtPlotGrid::enableXMin(bool tf)
{
    if ( d_data->xMinEnabled != tf )
    {
        d_data->xMinEnabled = tf;
        gridChanged();
    }
}

/*!
  \brief Enable or disable minor horizontal gridlines
  \param tf Enable (true) or disable
  \sa QwtPlotGrid::enableY()
*/
void QwtPlotGrid::enableYMin(bool tf)
{
    if ( d_data->yMinEnabled != tf )
    {
        d_data->yMinEnabled = tf;
        gridChanged();
    }
}

/*!
  \brief Assign an x axis scale division
  \param sx Scale division
  \warning QwtPlotGrid uses implicit sharing (see Qt Manual) for
  the scale divisions.
  \sa QwtScaleDiv
*/
void QwtPlotGrid::setXDiv(const QwtScaleDiv &sx)
{
    if ( d_data->sdx != sx )
    {
        d_data->sdx = sx;
        gridChanged();
    }
}

/*!
  \brief Assign a y axis division
  \param sy Scale division
  \warning QwtPlotGrid uses implicit sharing (see Qt Manual) for
  the scale divisions.
  \sa QwtScaleDiv
*/
void QwtPlotGrid::setYDiv(const QwtScaleDiv &sy)
{
    if ( d_data->sdy != sy )
    {
        d_data->sdy = sy;    
        gridChanged();
    }
}

/*!
  \brief Assign a pen for both major and minor gridlines
  \param p Pen
*/
void QwtPlotGrid::setPen(const QPen &p)
{
    if ( d_data->majPen != p || d_data->minPen != p )
    {
        d_data->majPen = p;
        d_data->minPen = p;
        gridChanged();
    }
}

/*!
  \brief Assign a pen for the major gridlines
  \param p Pen
*/
void QwtPlotGrid::setMajPen(const QPen &p)
{
    if ( d_data->majPen != p )
    {
        d_data->majPen = p;
        gridChanged();
    }
}

/*!
  \brief Assign a pen for the minor gridlines
  \param p Pen
*/
void QwtPlotGrid::setMinPen(const QPen &p)
{
    if ( d_data->minPen != p )
    {
        d_data->minPen = p;  
        gridChanged();
    }
}

/*!
  \brief Draw the grid
  
  The grid is drawn into the bounding rectangle such that 
  gridlines begin and end at the rectangle's borders. The X and Y
  maps are used to map the scale divisions into the drawing region
  screen.
  \param painter  Painter
  \param mx X axis map
  \param my Y axis 
  \param r Contents rect of the plot canvas
*/
void QwtPlotGrid::draw(QPainter *painter, 
    const QwtScaleMap &mx, const QwtScaleMap &my,
    const QRect &r) const
{
    //  draw minor gridlines
    painter->setPen(d_data->minPen);
    
    if (d_data->xEnabled && d_data->xMinEnabled)
    {
        drawLines(painter, r, Qt::Vertical, mx, 
            d_data->sdx.ticks(QwtScaleDiv::MinorTick));
        drawLines(painter, r, Qt::Vertical, mx, 
            d_data->sdx.ticks(QwtScaleDiv::MediumTick));
    }

    if (d_data->yEnabled && d_data->yMinEnabled)
    {
        drawLines(painter, r, Qt::Horizontal, my, 
            d_data->sdy.ticks(QwtScaleDiv::MinorTick));
        drawLines(painter, r, Qt::Horizontal, my, 
            d_data->sdy.ticks(QwtScaleDiv::MediumTick));
    }

    //  draw major gridlines
    painter->setPen(d_data->majPen);
    
    if (d_data->xEnabled)
    {
        drawLines(painter, r, Qt::Vertical, mx,
            d_data->sdx.ticks(QwtScaleDiv::MajorTick));
    }

    if (d_data->yEnabled)
    {
        drawLines(painter, r, Qt::Horizontal, my,
            d_data->sdy.ticks(QwtScaleDiv::MajorTick));
    }
}

void QwtPlotGrid::drawLines(QPainter *painter, const QRect &rect,
    Qt::Orientation orientation, const QwtScaleMap &map, 
    const QwtTickList &values) const
{
    const int x1 = rect.left();
    const int x2 = rect.right();
    const int y1 = rect.top();
    const int y2 = rect.bottom();

    for (uint i = 0; i < (uint)values.count(); i++)
    {
        const int value = map.transform(values[i]);
        if ( orientation == Qt::Horizontal )
        {
            if ((value >= y1) && (value <= y2))
                QwtPainter::drawLine(painter, x1, value, x2, value);
        }
        else
        {
            if ((value >= x1) && (value <= x2))
                QwtPainter::drawLine(painter, value, y1, value, y2);
        }
    }
}

/*!
  \return the pen for the major gridlines
  \sa QwtPlotGrid::setMinPen, QwtPlotGrid::setPen
*/
const QPen &QwtPlotGrid::majPen() const 
{ 
    return d_data->majPen; 
}

/*!
  \return the pen for the minor gridlines
  \sa QwtPlotGrid::setMinPen, QwtPlotGrid::setPen
*/
const QPen &QwtPlotGrid::minPen() const 
{ 
    return d_data->minPen; 
}
  
/*!
  \return true if vertical gridlines are enabled
  \sa QwtPlotGrid::enableX
*/
bool QwtPlotGrid::xEnabled() const
{ 
    return d_data->xEnabled; 
}

/*!
  \return true if minor vertical gridlines are enabled
  \sa QwtPlotGrid::enableXMin
*/
bool QwtPlotGrid::xMinEnabled() const 
{ 
    return d_data->xMinEnabled; 
}

/*!
  \return true if horizontal gridlines are enabled
  \sa QwtPlotGrid::enableY
*/
bool QwtPlotGrid::yEnabled() const 
{ 
    return d_data->yEnabled; 
}

/*!
  \return true if minor horizontal gridlines are enabled
  \sa QwtPlotGrid::enableYMin
*/
bool QwtPlotGrid::yMinEnabled() const 
{
    return d_data->yMinEnabled; 
}

  
/*! \return the scale division of the x axis */
const QwtScaleDiv &QwtPlotGrid::xScaleDiv() const 
{ 
    return d_data->sdx; 
}

/*! \return the scale division of the y axis */
const QwtScaleDiv &QwtPlotGrid::yScaleDiv() const 
{ 
    return d_data->sdy; 
}
 
void QwtPlotGrid::updateScaleDiv(const QwtScaleDiv& xDiv,
    const QwtScaleDiv& yDiv)
{
    setXDiv(xDiv);
    setYDiv(yDiv);
}

/*!
  \brief Notify a change of attributes

  This virtual function is called when an attribute of the grid
  has changed. It can be redefined by derived classes. The
  default implementation does nothing.
*/
void QwtPlotGrid::gridChanged() 
{
    itemChanged();
}

