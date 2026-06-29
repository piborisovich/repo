#ifndef PATHPROPERTIESWIDGET_HPP
#define PATHPROPERTIESWIDGET_HPP

#include "itempropertieswidget.hpp"
#include "pathitem.hpp"

class PathPropertiesWidget : public ItemPropertiesWidget
{
    Q_OBJECT
public:
    PathPropertiesWidget(PathItem* pathItem,
                         QWidget *parent = nullptr,
                         Qt::WindowFlags flags = Qt::WindowFlags());

private:
    PathItem* m_pathItem;
};

#endif // PATHPROPERTIESWIDGET_HPP
