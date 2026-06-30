#ifndef RECTPROPERTIESWIDGET_HPP
#define RECTPROPERTIESWIDGET_HPP

#include "itempropertieswidget.hpp"
#include <QGraphicsRectItem>

class RectPropertiesWidget : public ItemPropertiesWidget
{
    Q_OBJECT
public:
    RectPropertiesWidget(QGraphicsRectItem* rectItem,
                         QWidget *parent = nullptr,
                         Qt::WindowFlags flags = Qt::WindowFlags());

private:
    QGraphicsRectItem* m_rectItem;
};

#endif // RECTPROPERTIESWIDGET_HPP
