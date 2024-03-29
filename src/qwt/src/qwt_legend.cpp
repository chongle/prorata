/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

// vim: expandtab

#include <qapplication.h> 
#include <qmap.h> 
#if QT_VERSION >= 0x040000
#include <qscrollbar.h> 
#endif
#include "qwt_math.h"
#include "qwt_dyngrid_layout.h"
#include "qwt_plot_item.h"
#include "qwt_legend_item.h"
#include "qwt_legend.h"

class QwtLegend::PrivateData
{
public:
    class LegendMap
    {
    public:
        void insert(const QwtPlotItem *, QWidget *);

        void remove(const QwtPlotItem *);
        void remove(QWidget *);

        void clear();

        uint count() const;

        inline const QWidget *find(const QwtPlotItem *) const;
        inline QWidget *find(const QwtPlotItem *);

        inline const QwtPlotItem *find(const QWidget *) const;
        inline QwtPlotItem *find(const QWidget *);

        const QMap<QWidget *, const QwtPlotItem *> &widgetMap() const;
        QMap<QWidget *, const QwtPlotItem *> &widgetMap();

    private:
        QMap<QWidget *, const QwtPlotItem *> d_widgetMap;
        QMap<const QwtPlotItem *, QWidget *> d_itemMap;
    };

    QwtLegend::LegendItemMode itemMode;
    QwtLegend::LegendDisplayPolicy displayPolicy;
    int identifierMode;

    LegendMap map;

    class LegendView;
    LegendView *view;
};

#if QT_VERSION < 0x040000
#include <qscrollview.h>

class QwtLegend::PrivateData::LegendView: public QScrollView
{
public:
    LegendView(QWidget *parent):
        QScrollView(parent)
    {
        setResizePolicy(Manual);

        viewport()->setBackgroundMode(Qt::NoBackground); // Avoid flicker

        contentsWidget = new QWidget(viewport());

        addChild(contentsWidget);
    }

    void viewportResizeEvent(QResizeEvent *e)
    {
        QScrollView::viewportResizeEvent(e);

        // It's not safe to update the layout now, because
        // we are in an internal update of the scrollview framework.
        // So we delay the update by posting a LayoutHint.

        QApplication::postEvent(contentsWidget, 
            new QEvent(QEvent::LayoutHint));
    }

    QWidget *contentsWidget;
};

#else // QT_VERSION >= 0x040000

#include <qscrollarea.h>

class QwtLegend::PrivateData::LegendView: public QScrollArea
{
public:
    LegendView(QWidget *parent):
        QScrollArea(parent)
    {
        contentsWidget = new QWidget(this);

        setWidget(contentsWidget);
        setWidgetResizable(false);
    }

    virtual bool viewportEvent(QEvent *e) 
    {
        bool ok = QScrollArea::viewportEvent(e);

        if ( e->type() == QEvent::Resize )
        {
            QApplication::postEvent(contentsWidget, 
                new QEvent(QEvent::LayoutRequest));
        }
        return ok;
    }

    QSize viewportSize(int w, int h) const
    {
        const int sbHeight = horizontalScrollBar()->sizeHint().height();
        const int sbWidth = verticalScrollBar()->sizeHint().width();
    
        const int cw = contentsRect().width();
        const int ch = contentsRect().height();

        int vw = cw;
        int vh = ch;

        if ( w > vw )
            vh -= sbHeight;

        if ( h > vh )
        {
            vw -= sbWidth;
            if ( w > vw && vh == ch )
                vh -= sbHeight;
        }
        return QSize(vw, vh);
    }

    QWidget *contentsWidget;
};

#endif


void QwtLegend::PrivateData::LegendMap::insert(
    const QwtPlotItem *item, QWidget *widget)
{
    d_itemMap.insert(item, widget);
    d_widgetMap.insert(widget, item);
}

void QwtLegend::PrivateData::LegendMap::remove(const QwtPlotItem *item)
{
    QWidget *widget = d_itemMap[item];
    d_itemMap.remove(item);
    d_widgetMap.remove(widget);
}

void QwtLegend::PrivateData::LegendMap::remove(QWidget *widget)
{
    const QwtPlotItem *item = d_widgetMap[widget];
    d_itemMap.remove(item);
    d_widgetMap.remove(widget);
}

void QwtLegend::PrivateData::LegendMap::clear()
{
    QMap<QWidget *, const QwtPlotItem *>::const_iterator it;
    for ( it = d_widgetMap.begin(); it != d_widgetMap.end(); ++it ) 
        delete it.key();

    d_itemMap.clear();
    d_widgetMap.clear();
}

uint QwtLegend::PrivateData::LegendMap::count() const
{
    return d_itemMap.count();
}

inline const QWidget *QwtLegend::PrivateData::LegendMap::find(const QwtPlotItem *item) const
{
    if ( !d_itemMap.contains((QwtPlotItem *)item) )
        return NULL;

    return d_itemMap[(QwtPlotItem *)item];
}

inline QWidget *QwtLegend::PrivateData::LegendMap::find(const QwtPlotItem *item)
{
    if ( !d_itemMap.contains((QwtPlotItem *)item) )
        return NULL;

    return d_itemMap[(QwtPlotItem *)item];
}

inline const QwtPlotItem *QwtLegend::PrivateData::LegendMap::find(
    const QWidget *widget) const
{
    if ( !d_widgetMap.contains((QWidget *)widget) )
        return NULL;

    return d_widgetMap[(QWidget *)widget];
}

inline QwtPlotItem *QwtLegend::PrivateData::LegendMap::find(
    const QWidget *widget)
{
    if ( !d_widgetMap.contains((QWidget *)widget) )
        return NULL;

    return (QwtPlotItem *)d_widgetMap[(QWidget *)widget];
}

inline const QMap<QWidget *, const QwtPlotItem *> &
    QwtLegend::PrivateData::LegendMap::widgetMap() const
{
    return d_widgetMap;
} 

inline QMap<QWidget *, const QwtPlotItem *> &
    QwtLegend::PrivateData::LegendMap::widgetMap() 
{
    return d_widgetMap;
} 

/*!
  \param parent Parent widget
*/
QwtLegend::QwtLegend(QWidget *parent): 
    QFrame(parent)
{
    setFrameStyle(NoFrame);

    d_data = new QwtLegend::PrivateData;
    d_data->itemMode = QwtLegend::ReadOnlyItem;
    d_data->displayPolicy = QwtLegend::Auto;
    d_data->identifierMode = QwtLegendItem::ShowLine | 
        QwtLegendItem::ShowSymbol | QwtLegendItem::ShowText;

    d_data->view = new QwtLegend::PrivateData::LegendView(this);
    d_data->view->setFrameStyle(NoFrame);

    QwtDynGridLayout *layout = new QwtDynGridLayout(
        d_data->view->contentsWidget);
#if QT_VERSION < 0x040000
    layout->setAutoAdd(true);
#endif
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    d_data->view->contentsWidget->installEventFilter(this);
}

QwtLegend::~QwtLegend()
{
    delete d_data;
}

/*!
  Set the legend display policy to:

  \param policy Legend display policy
  \param mode Identifier mode (or'd ShowLine, ShowSymbol, ShowText)

  \sa QwtLegend::displayPolicy, QwtLegend::LegendDisplayPolicy
*/
void QwtLegend::setDisplayPolicy(LegendDisplayPolicy policy, int mode)
{
    d_data->displayPolicy = policy;
    if (-1 != mode)
       d_data->identifierMode = mode;

    QMap<QWidget *, const QwtPlotItem *> &map = 
        d_data->map.widgetMap();

    QMap<QWidget *, const QwtPlotItem *>::iterator it;
    for ( it = map.begin(); it != map.end(); ++it ) 
    {
#if QT_VERSION < 0x040000
        QwtPlotItem *item = (QwtPlotItem *)it.data();
#else
        QwtPlotItem *item = (QwtPlotItem *)it.value();
#endif
        if ( item )
            item->updateLegend(this);
    }
}

/*! 
  \return the legend display policy.
  Default is LegendDisplayPolicy::Auto.
  \sa QwtLegend::setDisplayPolicy, QwtLegend::LegendDisplayPolicy
*/ 

QwtLegend::LegendDisplayPolicy QwtLegend::displayPolicy() const 
{ 
    return d_data->displayPolicy; 
}

void QwtLegend::setItemMode(LegendItemMode mode)
{
    d_data->itemMode = mode;
}

QwtLegend::LegendItemMode QwtLegend::itemMode() const
{
    return d_data->itemMode;
}

/*!
  \return the IdentifierMode to be used in combination with
  LegendDisplayPolicy::Fixed.

  Default is ShowLine | ShowSymbol | ShowText.
*/

int QwtLegend::identifierMode() const
{
    return d_data->identifierMode;
}

/*! 
  The contents widget is the only child of the viewport() and
  the parent widget of all legend items.
*/
QWidget *QwtLegend::contentsWidget() 
{ 
    return d_data->view->contentsWidget; 
}

QScrollBar *QwtLegend::horizontalScrollBar() const
{
    return d_data->view->horizontalScrollBar();
}

QScrollBar *QwtLegend::verticalScrollBar() const
{
    return d_data->view->horizontalScrollBar();
}

/*!  
  The contents widget is the only child of the viewport() and
  the parent widget of all legend items.
*/

const QWidget *QwtLegend::contentsWidget() const 
{ 
    return d_data->view->contentsWidget; 
}

/*!
  Insert a new item for a plot item
  \param plotItem Plot item
  \param legendItem New legend item
  \note The parent of item will be changed to QwtLegend::contentsWidget()
*/
void QwtLegend::insert(const QwtPlotItem *plotItem, QWidget *legendItem)
{
    if ( legendItem == NULL || plotItem == NULL )
        return;

    QWidget *contentsWidget = d_data->view->contentsWidget;

    if ( legendItem->parent() != contentsWidget )
    {
#if QT_VERSION >= 0x040000
        legendItem->setParent(contentsWidget);
#else
        legendItem->reparent(contentsWidget, QPoint(0, 0));
#endif
    }

    legendItem->show();

    d_data->map.insert(plotItem, legendItem);

    layoutContents();

    if ( contentsWidget->layout() )
    {
#if QT_VERSION >= 0x040000
        contentsWidget->layout()->addWidget(legendItem);
#endif

        // set tab focus chain

        QWidget *w = NULL;

#if QT_VERSION < 0x040000
        QLayoutIterator layoutIterator = 
            contentsWidget->layout()->iterator();
        for ( QLayoutItem *item = layoutIterator.current();
            item != 0; item = ++layoutIterator)
        {
#else
        for (int i = 0; i < contentsWidget->layout()->count(); i++)
        {
            QLayoutItem *item = contentsWidget->layout()->itemAt(i);
#endif
            if ( w && item->widget() )
            {
                QWidget::setTabOrder(w, item->widget());
                w = item->widget();
            }
        }
    }
}

QWidget *QwtLegend::find(const QwtPlotItem *plotItem) const
{
    return d_data->map.find(plotItem);
}

QwtPlotItem *QwtLegend::find(const QWidget *legendItem) const
{
    return d_data->map.find(legendItem);
}

//! Find the corresponding item for a plotItem and remove it from the item list.
void QwtLegend::remove(const QwtPlotItem *plotItem)
{ 
    QWidget *legendItem = d_data->map.find(plotItem);
    d_data->map.remove(legendItem); 
    delete legendItem;
}

//! Remove all items.
void QwtLegend::clear()
{
#if QT_VERSION < 0x040000
    bool doUpdate = isUpdatesEnabled();
#else
    bool doUpdate = updatesEnabled();
#endif
    setUpdatesEnabled(false);

    d_data->map.clear();

    setUpdatesEnabled(doUpdate);
    update();
}

//! Return a size hint.
QSize QwtLegend::sizeHint() const
{
    QSize hint = d_data->view->contentsWidget->sizeHint();
    hint += QSize(2 * frameWidth(), 2 * frameWidth());

    return hint;
}

/*!
  \return The preferred height, for the width w.
*/
int QwtLegend::heightForWidth(int w) const
{
    w -= 2 * frameWidth();

    int h = d_data->view->contentsWidget->heightForWidth(w);
#if QT_VERSION < 0x040000

    // Asking the layout is the default implementation in Qt4 

    if ( h <= 0 ) 
    {
        QLayout *l = d_data->view->contentsWidget->layout();
        if ( l && l->hasHeightForWidth() )
            h = l->heightForWidth(w);
    }
#endif
    if ( h >= 0 )
        h += 2 * frameWidth();

    return h;
}

/*!
  Adjust contents widget and item layout to the size of the viewport().
*/
void QwtLegend::layoutContents()
{
    const QSize visibleSize = d_data->view->viewport()->size();

    const QLayout *l = d_data->view->contentsWidget->layout();
    if ( l && l->inherits("QwtDynGridLayout") )
    {
        const QwtDynGridLayout *tl = (const QwtDynGridLayout *)l;

        const int minW = int(tl->maxItemWidth()) + 2 * tl->margin();

        int w = qwtMax(visibleSize.width(), minW);
        int h = qwtMax(tl->heightForWidth(w), visibleSize.height());

        const int vpWidth = d_data->view->viewportSize(w, h).width();
        if ( w > vpWidth )
        {
            w = qwtMax(vpWidth, minW);
            h = qwtMax(tl->heightForWidth(w), visibleSize.height());
        }

        d_data->view->contentsWidget->resize(w, h);
#if QT_VERSION < 0x040000
        d_data->view->resizeContents(w, h);
#endif
    }
}

/*
  Filter layout related events of QwtLegend::contentsWidget().
*/

bool QwtLegend::eventFilter(QObject *o, QEvent *e)
{
    if ( o == d_data->view->contentsWidget )
    {
        switch(e->type())
        {
            case QEvent::ChildRemoved:
            {   
                const QChildEvent *ce = (const QChildEvent *)e;
                if ( ce->child()->isWidgetType() )
                    d_data->map.remove((QWidget *)ce->child());
                break;
            }
#if QT_VERSION < 0x040000
            case QEvent::LayoutHint:
#else
            case QEvent::LayoutRequest:
#endif
            {
                layoutContents();
                break;
            }
#if QT_VERSION < 0x040000
            case QEvent::Resize:
            {
                updateGeometry();
                break;
            }
#endif
            default:
                break;
        }
    }
    
    return QFrame::eventFilter(o, e);
}


//! Return true, if there are no legend items.
bool QwtLegend::isEmpty() const
{
    return d_data->map.count() == 0;
}

//! Return the number of legend items.
uint QwtLegend::itemCount() const
{
    return d_data->map.count();
}

#if QT_VERSION < 0x040000
QValueList<QWidget *> QwtLegend::legendItems() const
#else
QList<QWidget *> QwtLegend::legendItems() const
#endif
{
    const QMap<QWidget *, const QwtPlotItem *> &map = 
        d_data->map.widgetMap();

#if QT_VERSION < 0x040000
    QValueList<QWidget *> list;
#else
    QList<QWidget *> list;
#endif

    QMap<QWidget *, const QwtPlotItem *>::const_iterator it;
    for ( it = map.begin(); it != map.end(); ++it ) 
        list += it.key();

    return list;
}

void QwtLegend::resizeEvent(QResizeEvent *e)
{
    QFrame::resizeEvent(e);
    d_data->view->setGeometry(contentsRect());
}
