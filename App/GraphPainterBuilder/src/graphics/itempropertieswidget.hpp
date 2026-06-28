#ifndef ITEMPROPERTIESWIDGET_HPP
#define ITEMPROPERTIESWIDGET_HPP

#include <QWidget>

class ItemPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    ItemPropertiesWidget(QWidget* parent = nullptr,
                         Qt::WindowFlags f = Qt::WindowFlags());

    virtual void setColor(const QColor &color) = 0;
    virtual QColor color() const = 0;

};

#endif // ITEMPROPERTIESWIDGET_HPP
