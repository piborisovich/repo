#ifndef PROPERTIESDOCKWIDGET_HPP
#define PROPERTIESDOCKWIDGET_HPP

#include "itempropertieswidget.hpp"

#include <QDockWidget>
#include <QGraphicsItem>
#include <QPushButton>

class PropertiesDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    PropertiesDockWidget(const QString &title,
                         QGraphicsItem* graphicsItem,
                         QWidget *parent = nullptr,
                         Qt::WindowFlags flags = Qt::WindowFlags());

protected:
    void setColor(const QColor &color);

private Q_SLOTS:
    void on_colorClicked();

private:
    static ItemPropertiesWidget* createContent(QGraphicsItem* item);

private:
    QPushButton *m_colorButton;
    ItemPropertiesWidget *m_propertiesWidget;
};

#endif // PROPERTIESDOCKWIDGET_HPP
