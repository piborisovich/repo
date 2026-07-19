#include "pathpropertieswidget.hpp"

#include <QPen>

PathPropertiesWidget::PathPropertiesWidget(PathItem *pathItem,
                                           QWidget *parent,
                                           Qt::WindowFlags flags)
    : ItemPropertiesWidget(parent, flags)
    , m_pathItem(pathItem)
{
    setPen( m_pathItem->pen() );

    auto result = connect(this,
                          &PathPropertiesWidget::penChanged,
                          this,
                          [pathItem](const QPen &pen){
                              pathItem->setPen(pen);
                          });
    Q_ASSERT(result);
}
