#ifndef LINEPROPERTIESWIDGET_HPP
#define LINEPROPERTIESWIDGET_HPP

#include "itempropertieswidget.hpp"
#include "lineitem.hpp"

class LinePropertiesWidget : public ItemPropertiesWidget
{
    Q_OBJECT
public:
    LinePropertiesWidget(LineItem* lineItem,
                         QWidget *parent = nullptr,
                         Qt::WindowFlags flags = Qt::WindowFlags());

private:
    LineItem* m_lineItem;

};

#endif // LINEPROPERTIESWIDGET_HPP
