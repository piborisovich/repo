#include "linepropertieswidget.hpp"

#include <QPen>

LinePropertiesWidget::LinePropertiesWidget(LineItem *lineItem,
                                           QWidget *parent,
                                           Qt::WindowFlags flags)
    : ItemPropertiesWidget(parent, flags)
    , m_lineItem(lineItem)
{
    setPen( m_lineItem->pen() );

    auto result = connect(this,
                          &LinePropertiesWidget::penChanged,
                          this,
                          [lineItem](const QPen &pen){
                              lineItem->setPen(pen);
                          });
    Q_ASSERT(result);
}
