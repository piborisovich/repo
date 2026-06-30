#include "rectpropertieswidget.hpp"

#include <QPen>

RectPropertiesWidget::RectPropertiesWidget(QGraphicsRectItem* rectItem,
                                           QWidget *parent,
                                           Qt::WindowFlags flags)
    : ItemPropertiesWidget(parent, flags)
    , m_rectItem(rectItem)
{
    setPen( m_rectItem->pen() );

    auto result = connect(this,
                          &RectPropertiesWidget::penChanged,
                          this,
                          [rectItem](const QPen &pen){
                              rectItem->setPen(pen);
                          });
    Q_ASSERT(result);
}
