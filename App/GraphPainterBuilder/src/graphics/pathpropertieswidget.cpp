#include "pathpropertieswidget.hpp"

PathPropertiesWidget::PathPropertiesWidget(PathItem *pathItem,
                                           QWidget *parent,
                                           Qt::WindowFlags flags)
    : ItemPropertiesWidget(parent, flags)
    , m_pathItem(pathItem)
{
}

void PathPropertiesWidget::setColor(const QColor &color)
{

}

QColor PathPropertiesWidget::color() const
{
    return QColor();
}
