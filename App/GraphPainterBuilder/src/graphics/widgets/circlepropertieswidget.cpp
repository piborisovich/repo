#include "circlepropertieswidget.hpp"

#include <QPen>

CirclePropertiesWidget::CirclePropertiesWidget(QGraphicsEllipseItem *ellipseItem,
                                               QWidget *parent,
                                               Qt::WindowFlags flags)
: ItemPropertiesWidget(parent, flags)
    , m_ellipseItem(ellipseItem)
{
    setPen( m_ellipseItem->pen() );

    auto result = connect(this,
                          &CirclePropertiesWidget::penChanged,
                          this,
                          [ellipseItem](const QPen &pen){
                              ellipseItem->setPen(pen);
                          });
    Q_ASSERT(result);
}
