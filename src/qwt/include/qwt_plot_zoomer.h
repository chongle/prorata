/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

// vim: expandtab

#ifndef QWT_PLOT_ZOOMER_H
#define QWT_PLOT_ZOOMER_H

#include <qglobal.h>
#if QT_VERSION < 0x040000
#include <qvaluestack.h>
#else
#include <qstack.h>
#endif

#include "qwt_double_rect.h"
#include "qwt_plot_picker.h"

/*!
  \brief QwtPlotZoomer provides stacked zooming for a plot widget

  QwtPlotZoomer offers rubberband selections on the plot canvas, 
  translating the selected rectangles into plot coordinates and
  adjusting the axes to them. Zooming can repeated as often as 
  possible, limited only by maxStackDepth() or minZoomSize(). 
  Each rectangle is pushed on a stack. 

  Zoom rectangles can be selected depending on selectionFlags() using the
  mouse or keyboard (QwtEventPattern, QwtPickerMachine).
  QwtEventPattern::MouseSelect3/QwtEventPattern::KeyUndo, 
  or QwtEventPattern::MouseSelect6/QwtEventPattern::KeyRedo 
  walk up and down the zoom stack.
  QwtEventPattern::MouseSelect2 or QwtEventPattern::KeyHome unzoom to
  the initial size. 
  
  QwtPlotZoomer is tailored for plots with one x and y axis, but it is
  allowed to attach a second QwtPlotZoomer for the other axes.

  \note The realtime example includes an derived zoomer class that adds 
        scrollbars to the plot canvas.

  \warning Calling QwtPlot::setAxisScale() while QwtPlot::autoReplot() is false
           leaves the axis in an 'intermediate' state.
           In this case, to prevent buggy behaviour, your must call
       QwtPlot::replot() before calling QwtPlotPicker::scaleRect(),
       QwtPlotZoomer::scaleRect(), QwtPlotPicker::QwtPlotPicker() or
       QwtPlotZoomer::QwtPlotZoomer().
           This quirk will be removed in a future release.
*/

class QWT_EXPORT QwtPlotZoomer: public QwtPlotPicker
{
    Q_OBJECT
public:
    explicit QwtPlotZoomer(QwtPlotCanvas *);
    explicit QwtPlotZoomer(int xAxis, int yAxis, QwtPlotCanvas *);
    explicit QwtPlotZoomer(int xAxis, int yAxis, int selectionFlags,
        DisplayMode trackerMode, QwtPlotCanvas *);

    virtual ~QwtPlotZoomer();

    virtual void setZoomBase();
    virtual void setZoomBase(const QwtDoubleRect &);

    QwtDoubleRect zoomBase() const;
    QwtDoubleRect zoomRect() const;

    virtual void setAxis(int xAxis, int yAxis);

    void setMaxStackDepth(int);
    int maxStackDepth() const;

#if QT_VERSION < 0x040000
    const QValueStack<QwtDoubleRect> &zoomStack() const;
#else
    const QStack<QwtDoubleRect> &zoomStack() const;
#endif
    uint zoomRectIndex() const;

    virtual void setSelectionFlags(int);

public slots:
    void moveBy(double x, double y);
    virtual void move(double x, double y);

    virtual void zoom(const QwtDoubleRect &);
    virtual void zoom(int up);

signals:
    /*!
      A signal emitting the zoomRect(), when the plot has been 
      zoomed in or out.

      \param rect Current zoom rectangle.
      \warning zoomed() is only emitted after selections in end(), but never
               in zoom().
    */

    void zoomed(const QwtDoubleRect &rect);

protected:
    virtual void rescale();

    virtual QwtDoubleSize minZoomSize() const;

    virtual void widgetMouseReleaseEvent(QMouseEvent *);
    virtual void widgetKeyPressEvent(QKeyEvent *);

    virtual void begin();
    virtual bool end(bool ok = true);
    virtual bool accept(SelectedPoints &) const;

private:
    void init(int selectionFlags = RectSelection & ClickSelection, 
        DisplayMode trackerMode = ActiveOnly);

    class PrivateData;
    PrivateData *d_data;
};
            
#endif
