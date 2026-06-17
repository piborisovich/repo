#include "linepropertieswidget.hpp"

#include <QPen>

LinePropertiesWidget::LinePropertiesWidget(QGraphicsLineItem *lineItem,
                                           QWidget *parent,
                                           Qt::WindowFlags flags)
    : ItemPropertiesWidget(parent, flags)
    , m_lineItem(lineItem)
{

}

void LinePropertiesWidget::setColor(const QColor &color)
{
    QPen pen = m_lineItem->pen();
    pen.setColor(color);
    m_lineItem->setPen(pen);
}

QColor LinePropertiesWidget::color() const
{
    return m_lineItem->pen().color();
}
