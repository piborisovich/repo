#ifndef CIRCLEPROPERTIESWIDGET_HPP
#define CIRCLEPROPERTIESWIDGET_HPP

#include "itempropertieswidget.hpp"
#include <QGraphicsEllipseItem>

class CirclePropertiesWidget : public ItemPropertiesWidget
{
    Q_OBJECT
public:
    CirclePropertiesWidget(QGraphicsEllipseItem* ellipseItem,
                           QWidget *parent = nullptr,
                           Qt::WindowFlags flags = Qt::WindowFlags());

private:
    QGraphicsEllipseItem* m_ellipseItem;
};

#endif // CIRCLEPROPERTIESWIDGET_HPP
