/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PLOT_GRID_H
#define QWT_PLOT_GRID_H

#include "qwt_global.h"
#include "qwt_plot_item.h"
#include "qwt_scale_div.h"

class QPainter;
class QPen;
class QwtScaleMap;
class QwtScaleDiv;

/*!
  \brief A class which draws a coordinate grid

  The QwtPlotGrid class can be used to draw a coordinate grid.
  A coordinate grid consists of major and minor vertical
  and horizontal gridlines. The locations of the gridlines
  are determined by the X and Y scale divisions which can
  be assigned with QwtPlotGrid::setXDiv and QwtPlotGrid::setYDiv()
  The draw() member draws the grid within a bounding
  rectangle.
*/

class QWT_EXPORT QwtPlotGrid: public QwtPlotItem
{
public:
    explicit QwtPlotGrid();
    virtual ~QwtPlotGrid();

    virtual int rtti() const;

    void enableX(bool tf);
    bool xEnabled() const;

    void enableY(bool tf);
    bool yEnabled() const;

    void enableXMin(bool tf);
    bool xMinEnabled() const;

    void enableYMin(bool tf);
    bool yMinEnabled() const;

    void setXDiv(const QwtScaleDiv &sx);
    const QwtScaleDiv &xScaleDiv() const;

    void setYDiv(const QwtScaleDiv &sy);
    const QwtScaleDiv &yScaleDiv() const;

    void setPen(const QPen &p);

    void setMajPen(const QPen &p);
    const QPen& majPen() const;

    void setMinPen(const QPen &p);
    const QPen& minPen() const;

    virtual void draw(QPainter *p, 
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRect &rect) const;

    virtual void updateScaleDiv(const QwtScaleDiv&,
        const QwtScaleDiv&);

protected:
    virtual void gridChanged();

private:
    void drawLines(QPainter *painter, const QRect &,
        Qt::Orientation orientation, const QwtScaleMap &, 
        const QwtTickList &) const;

    class PrivateData;
    PrivateData *d_data;
};

#endif
