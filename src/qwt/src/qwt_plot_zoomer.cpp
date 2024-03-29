/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

// vim: expandtab

#include <math.h>
#include "qwt_plot.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_zoomer.h"
#include "qwt_scale_div.h"
#if QT_VERSION < 0x040000
typedef QValueStack<QwtDoubleRect> QwtZoomStack;
#else
typedef QStack<QwtDoubleRect> QwtZoomStack;
#endif

class QwtPlotZoomer::PrivateData
{
public:
    uint zoomRectIndex;
    QwtZoomStack zoomStack;

    int maxStackDepth;
};

/*!
  \brief Create a zoomer for a plot canvas.

  The zoomer is set to those x- and y-axis of the parent plot of the
  canvas that are enabled. If both or no x-axis are enabled, the picker
  is set to QwtPlot::xBottom. If both or no y-axis are
  enabled, it is set to QwtPlot::yLeft.

  The selectionFlags() are set to 
  QwtPicker::RectSelection & QwtPicker::ClickSelection, the
  tracker mode to QwtPicker::ActiveOnly.

  \param canvas Plot canvas to observe, also the parent object

  \warning Calling QwtPlot::setAxisScale() while QwtPlot::autoReplot() is false
           leaves the axis in an 'intermediate' state.
           In this case, to prevent buggy behaviour, you must call
           QwtPlot::replot() before calling QwtPlotZoomer().
           This quirk will be removed in a future release.

  \sa QwtPlot::autoReplot(), QwtPlot::replot(), QwtPlotPicker::setZoomBase()
*/
QwtPlotZoomer::QwtPlotZoomer(QwtPlotCanvas *canvas):
    QwtPlotPicker(canvas)
{
    if ( canvas )
        init();
}

/*!
  \brief Create a zoomer for a plot canvas.

  The selectionFlags() are set to 
  QwtPicker::RectSelection & QwtPicker::ClickSelection, the
  tracker mode to QwtPicker::ActiveOnly. 

  \param xAxis X axis of the zoomer
  \param yAxis Y axis of the zoomer
  \param canvas Plot canvas to observe, also the parent object

  \warning Calling QwtPlot::setAxisScale() while QwtPlot::autoReplot() is false
           leaves the axis in an 'intermediate' state.
           In this case, to prevent buggy behaviour, you must call
           QwtPlot::replot() before calling QwtPlotZoomer().
           This quirk will be removed in a future release.

  \sa QwtPlot::autoReplot(), QwtPlot::replot(), QwtPlotPicker::setZoomBase()
*/

QwtPlotZoomer::QwtPlotZoomer(int xAxis, int yAxis,
        QwtPlotCanvas *canvas):
    QwtPlotPicker(xAxis, yAxis, canvas)
{
    if ( canvas )
    {
        init();
    }
}

/*!
  Create a zoomer for a plot canvas.

  \param xAxis X axis of the zoomer
  \param yAxis Y axis of the zoomer
  \param selectionFlags Or�d value of QwtPicker::RectSelectionType and
                        QwtPicker::SelectionMode. 
                        QwtPicker::RectSelection will be auto added.
  \param trackerMode Tracker mode
  \param canvas Plot canvas to observe, also the parent object

  \sa QwtPicker, QwtPicker::setSelectionFlags(), QwtPicker::setRubberBand(),
      QwtPicker::setTrackerMode

  \warning Calling QwtPlot::setAxisScale() while QwtPlot::autoReplot() is false
           leaves the axis in an 'intermediate' state.
           In this case, to prevent buggy behaviour, you must call
           QwtPlot::replot() before calling QwtPlotZoomer().
           This quirk will be removed in a future release.

  \sa QwtPlot::autoReplot(), QwtPlot::replot(), QwtPlotPicker::setZoomBase()
*/

QwtPlotZoomer::QwtPlotZoomer(int xAxis, int yAxis, int selectionFlags,
        DisplayMode trackerMode, QwtPlotCanvas *canvas):
    QwtPlotPicker(xAxis, yAxis,canvas)
{
    if ( canvas )
    {
        init(selectionFlags, trackerMode);
    }
}

//! Init the zoomer, used by the constructors
void QwtPlotZoomer::init(int selectionFlags, DisplayMode trackerMode)
{
    d_data = new PrivateData;

    d_data->maxStackDepth = -1;

    setSelectionFlags(selectionFlags);
    setTrackerMode(trackerMode);
    setRubberBand(RectRubberBand);

    setZoomBase(scaleRect());
}

QwtPlotZoomer::~QwtPlotZoomer()
{
    delete d_data;
}

/*!
  \brief Limit the number of recursive zoom operations to depth.

  A value of -1 set the depth to unlimited, 0 disables zooming.
  If the current zoom rectangle is below depth, the plot is unzoomed.

  \param depth Maximum for the stack depth
  \sa maxStackDepth()
  \note depth doesn�t include the zoom base, so zoomStack().count() might be
              maxStackDepth() + 1.
*/
void QwtPlotZoomer::setMaxStackDepth(int depth)
{
    d_data->maxStackDepth = depth;

    if ( depth >= 0 )
    {
        // unzoom if the current depth is below d_data->maxStackDepth

        const int zoomOut = 
            int(d_data->zoomStack.count()) - 1 - depth; // -1 for the zoom base

        if ( zoomOut > 0 )
        {
            zoom(-zoomOut);
            for ( int i = int(d_data->zoomStack.count()) - 1; 
                i > int(d_data->zoomRectIndex); i-- )
            {
                (void)d_data->zoomStack.pop(); // remove trailing rects
            }
        }
    }
}

/*!
  \return Maximal depth of the zoom stack.
  \sa setMaxStackDepth()
*/
int QwtPlotZoomer::maxStackDepth() const
{
    return d_data->maxStackDepth;
}

/*!
  Return the zoom stack. zoomStack()[0] is the zoom base,
  zoomStack()[1] the first zoomed rectangle.
*/
const QwtZoomStack &QwtPlotZoomer::zoomStack() const
{
    return d_data->zoomStack;
}

/*!
  \return Initial rectangle of the zoomer
  \sa setZoomBase(), zoomRect()
*/
QwtDoubleRect QwtPlotZoomer::zoomBase() const
{
    return d_data->zoomStack[0];
}

/*!
  Reinitialized the zoom stack with scaleRect() as base.

  \sa zoomBase(), scaleRect()

  \warning Calling QwtPlot::setAxisScale() while QwtPlot::autoReplot() is false
           leaves the axis in an 'intermediate' state.
           In this case, to prevent buggy behaviour, you must call
           QwtPlot::replot() before calling QwtPlotZoomer::setZoomBase().
           This quirk will be removed in a future release.

  \sa QwtPlot::autoReplot(), QwtPlot::replot().
*/
void QwtPlotZoomer::setZoomBase()
{
    const QwtPlot *plt = plot();
    if ( !plt )
        return;

    d_data->zoomStack.clear();
    d_data->zoomStack.push(scaleRect());
    d_data->zoomRectIndex = 0;

    rescale();
}

/*!
  \brief Set the initial size of the zoomer.

  base is united with the current scaleRect() and the zoom stack is
  reinitalized with it as zoom base. plot is zoomed to scaleRect().
  
  \param base Zoom base
  
  \sa zoomBase(), scaleRect()
*/
void QwtPlotZoomer::setZoomBase(const QwtDoubleRect &base)
{
    const QwtPlot *plt = plot();
    if ( !plt )
        return;

    const QwtDoubleRect sRect = scaleRect();
    const QwtDoubleRect bRect = base | sRect;

    d_data->zoomStack.clear();
    d_data->zoomStack.push(bRect);
    d_data->zoomRectIndex = 0;

    if ( base != sRect )
    {
        d_data->zoomStack.push(sRect);
        d_data->zoomRectIndex++;
    }

    rescale();
}

/*! 
  Rectangle at the current position on the zoom stack. 

  \sa QwtPlotZoomer::zoomRectIndex(), QwtPlotZoomer::scaleRect().
*/
QwtDoubleRect QwtPlotZoomer::zoomRect() const
{
    return d_data->zoomStack[d_data->zoomRectIndex];
}

/*! 
  \return Index of current position of zoom stack.
*/
uint QwtPlotZoomer::zoomRectIndex() const
{
    return d_data->zoomRectIndex;
}

/*!
  \brief Zoom in

  Clears all rectangles above the current position of the
  zoom stack and pushs the intersection of zoomRect() and 
  the normalized rect on it.

  \note If the maximal stack depth is reached, zoom is ignored.
*/

void QwtPlotZoomer::zoom(const QwtDoubleRect &rect)
{   
    if ( d_data->maxStackDepth >= 0 && 
        int(d_data->zoomRectIndex) >= d_data->maxStackDepth )
    {
        return;
    }

    const QwtDoubleRect zoomRect = d_data->zoomStack[0] & rect.normalized();
    if ( zoomRect != d_data->zoomStack[d_data->zoomRectIndex] )
    {
        for ( uint i = int(d_data->zoomStack.count()) - 1; 
           i > d_data->zoomRectIndex; i-- )
        {
            (void)d_data->zoomStack.pop();
        }

        d_data->zoomStack.push(zoomRect);
        d_data->zoomRectIndex++;

        rescale();
    }
}

/*!
  \brief Zoom in or out

  Activate a rectangle on the zoom stack with an offset relative
  to the current position. Negative values of offest will zoom out,
  positive zoom in. A value of 0 zooms out to the zoom base.

  \param offset Offset relative to the current position of the zoom stack.
  \sa zoomRectIndex()
*/
void QwtPlotZoomer::zoom(int offset)
{
    if ( offset == 0 )
        d_data->zoomRectIndex = 0;
    else
    {
        int newIndex = d_data->zoomRectIndex + offset;
        newIndex = qwtMax(0, newIndex);
        newIndex = qwtMin(int(d_data->zoomStack.count()) - 1, newIndex);

        d_data->zoomRectIndex = uint(newIndex);
    }

    rescale();
}

/*! 
  Adjust the observed plot to zoomRect()

  \note Initiates QwtPlot::replot
*/

void QwtPlotZoomer::rescale()
{
    QwtPlot *plt = plot();
    if ( !plt )
        return;

    const QwtDoubleRect &rect = d_data->zoomStack[d_data->zoomRectIndex];
    if ( rect != scaleRect() )
    {
        const bool doReplot = plt->autoReplot();
        plt->setAutoReplot(false);

        double x1 = rect.left();
        double x2 = rect.right();
        if ( plt->axisScaleDiv(xAxis())->lBound() > 
            plt->axisScaleDiv(xAxis())->hBound() )
        {
            qSwap(x1, x2);
        }

        plt->setAxisScale(xAxis(), x1, x2);

        double y1 = rect.top();
        double y2 = rect.bottom();
        if ( plt->axisScaleDiv(yAxis())->lBound() > 
            plt->axisScaleDiv(yAxis())->hBound() )
        {
            qSwap(y1, y2);
        }
        plt->setAxisScale(yAxis(), y1, y2);

        plt->setAutoReplot(doReplot);

        plt->replot();
    }
}

/*!
  Reinitialize the axes, and set the zoom base to their scales.

  \param xAxis X axis 
  \param yAxis Y axis
*/

void QwtPlotZoomer::setAxis(int xAxis, int yAxis)
{
    if ( xAxis != QwtPlotPicker::xAxis() || yAxis != QwtPlotPicker::yAxis() )
    {
        QwtPlotPicker::setAxis(xAxis, yAxis);
        setZoomBase(scaleRect());
    }
}

/*!
   Qt::MidButton zooms out one position on the zoom stack,
   Qt::RightButton to the zoom base.

   Changes the current position on the stack, but doesn�t pop
   any rectangle.

   \note The mouse events can be changed, using
         QwtEventPattern::setMousePattern: 2, 1
*/
void QwtPlotZoomer::widgetMouseReleaseEvent(QMouseEvent *me)
{
    if ( mouseMatch(MouseSelect2, me) )
        zoom(0);
    else if ( mouseMatch(MouseSelect3, me) )
        zoom(-1);
    else if ( mouseMatch(MouseSelect6, me) )
        zoom(+1);
    else 
        QwtPlotPicker::widgetMouseReleaseEvent(me);
}

/*!
   Qt::Key_Plus zooms out, Qt::Key_Minus zooms in one position on the 
   zoom stack, Qt::Key_Escape zooms out to the zoom base.

   Changes the current position on the stack, but doesn�t pop
   any rectangle.

   \note The keys codes can be changed, using
         QwtEventPattern::setKeyPattern: 3, 4, 5
*/

void QwtPlotZoomer::widgetKeyPressEvent(QKeyEvent *ke)
{
    if ( !isActive() )
    {
        if ( keyMatch(KeyUndo, ke) )
            zoom(-1);
        else if ( keyMatch(KeyRedo, ke) )
            zoom(+1);
        else if ( keyMatch(KeyHome, ke) )
            zoom(0);
    }

    QwtPlotPicker::widgetKeyPressEvent(ke);
}

/*!
  Move the current zoom rectangle.

  \param dx X offset
  \param dy Y offset

  \note The changed rectangle is limited by the zoom base
*/
void QwtPlotZoomer::moveBy(double dx, double dy)
{
    const QwtDoubleRect &rect = d_data->zoomStack[d_data->zoomRectIndex];
    move(rect.left() + dx, rect.top() + dy);
}

/*!
  Move the the current zoom rectangle.

  \param x X value
  \param y Y value

  \sa QwtDoubleRect::move
  \note The changed rectangle is limited by the zoom base
*/
void QwtPlotZoomer::move(double x, double y)
{
    x = qwtMax(x, zoomBase().left());
    x = qwtMin(x, zoomBase().right() - zoomRect().width());

    y = qwtMax(y, zoomBase().top());
    y = qwtMin(y, zoomBase().bottom() - zoomRect().height());

    if ( x != zoomRect().left() || y != zoomRect().top() )
    {
        d_data->zoomStack[d_data->zoomRectIndex].moveTo(x, y);
        rescale();
    }
}

/*!
  \brief Check and correct a selected rectangle

  Reject rectangles with a hight or width < 2, otherwise
  expand the selected rectangle to a minimum size of 11x11
  and accept it.
  
  \return true If rect is accepted, or has been changed
          to a accepted rectangle. 
*/

bool QwtPlotZoomer::accept(SelectedPoints &pa) const
{
    if ( pa.count() < 2 )
        return false;

    QRect rect = QRect(pa[0], pa[int(pa.count()) - 1]);
#if QT_VERSION < 0x040000
    rect = rect.normalize();
#else
    rect = rect.normalized();
#endif

    const int minSize = 2;
    if (rect.width() < minSize && rect.height() < minSize )
        return false; 

    const int minZoomSize = 11;

    const QPoint center = rect.center();
    rect.setSize(rect.size().expandedTo(QSize(minZoomSize, minZoomSize)));
    rect.moveCenter(center);

    pa.resize(2);
    pa[0] = rect.topLeft();
    pa[1] = rect.bottomRight();

    return true;
}

/*!
  \brief Limit zooming by a minimum rectangle

  \return zoomBase().width() / 10e4, zoomBase().height() / 10e4
*/
QwtDoubleSize QwtPlotZoomer::minZoomSize() const
{
    return QwtDoubleSize(
        d_data->zoomStack[0].width() / 10e4,
        d_data->zoomStack[0].height() / 10e4
    );
}

/*! 
  Rejects selections, when the stack depth is too deep, or
  the zoomed rectangle is minZoomSize().

  \sa minZoomSize(), maxStackDepth()
*/
void QwtPlotZoomer::begin()
{
    if ( d_data->maxStackDepth >= 0 )
    {
        if ( d_data->zoomRectIndex >= uint(d_data->maxStackDepth) )
            return;
    }

    const QwtDoubleSize minSize = minZoomSize();
    if ( minSize.isValid() )
    {
        const QwtDoubleSize sz = 
            d_data->zoomStack[d_data->zoomRectIndex].size() * 0.9999;

        if ( minSize.width() >= sz.width() &&
            minSize.height() >= sz.height() )
        {
            return;
        }
    }

    QwtPlotPicker::begin();
}

/*!
  Expand the selected rectangle to minZoomSize() and zoom in
  if accepted.

  \note The zoomed signal is emitted.
  \sa QwtPlotZoomer::accept()a, QwtPlotZoomer::minZoomSize()
*/
bool QwtPlotZoomer::end(bool ok)
{
    ok = QwtPlotPicker::end(ok);
    if (!ok)
        return false;

    QwtPlot *plot = QwtPlotZoomer::plot();
    if ( !plot )
        return false;

    const SelectedPoints &pa = selection();
    if ( pa.count() < 2 )
        return false;

    QRect rect = QRect(pa[0], pa[int(pa.count() - 1)]);
#if QT_VERSION < 0x040000
    rect = rect.normalize();
#else
    rect = rect.normalized();
#endif


    QwtDoubleRect zoomRect = invTransform(rect).normalized();

    const QwtDoublePoint center = zoomRect.center();
    zoomRect.setSize(zoomRect.size().expandedTo(minZoomSize()));
    zoomRect.moveCenter(center);

    zoom(zoomRect);
    emit zoomed(zoomRect);

    return true;
}

/*!
  Set the selection flags
  
  \param flags Or�d value of QwtPicker::RectSelectionType and
               QwtPicker::SelectionMode. The default value is 
               QwtPicker::RectSelection & QwtPicker::ClickSelection.

  \sa selectionFlags(), SelectionType, RectSelectionType, SelectionMode
  \note QwtPicker::RectSelection will be auto added.
*/

void QwtPlotZoomer::setSelectionFlags(int flags)
{
    // we accept only rects
    flags &= ~(PointSelection | PolygonSelection);
    flags |= RectSelection;

    QwtPlotPicker::setSelectionFlags(flags);
}
