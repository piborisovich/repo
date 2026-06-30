#include "propertiesdockwidget.hpp"
#include "linepropertieswidget.hpp"
#include "pathpropertieswidget.hpp"
#include "circlepropertieswidget.hpp"
#include "rectpropertieswidget.hpp"

#include "lineitem.hpp"
#include "pathitem.hpp"
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>

#include <QVBoxLayout>
#include <QColorDialog>

PropertiesDockWidget::PropertiesDockWidget(const QString &title,
                                           QGraphicsItem *graphicsItem,
                                           QWidget *parent,
                                           Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags)
    , m_propertiesWidget( createContent(graphicsItem) )
{
    setFeatures(QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable);

    QWidget *contentWidget = new QWidget(this);

    QVBoxLayout* widgetLayout = new QVBoxLayout(contentWidget);

    widgetLayout->setContentsMargins(5, 5, 5, 5);
    widgetLayout->addWidget(m_propertiesWidget);
    widgetLayout->addStretch();

    setWidget( contentWidget );
}

ItemPropertiesWidget *PropertiesDockWidget::createContent(QGraphicsItem *item)
{
    LineItem* lineItem =  dynamic_cast<LineItem*>(item);

    if ( lineItem ) {
        return new LinePropertiesWidget(lineItem);
    }

    PathItem* pathItem =  dynamic_cast<PathItem*>(item);

    if ( pathItem ) {
        return new PathPropertiesWidget(pathItem);
    }

    QGraphicsEllipseItem* ellipseItem =  dynamic_cast<QGraphicsEllipseItem*>(item);

    if ( ellipseItem ) {
        return new CirclePropertiesWidget(ellipseItem);
    }

    QGraphicsRectItem* rectItem =  dynamic_cast<QGraphicsRectItem*>(item);

    if ( rectItem ) {
        return new RectPropertiesWidget(rectItem);
    }

    return nullptr;
}
