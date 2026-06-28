#ifndef TOOLWIDGET_HPP
#define TOOLWIDGET_HPP

#include "toollist.hpp"

#include <QMap>
#include <QWidget>

/*!
 * \brief Отображение инструментов
 */
class ToolWidget : public QWidget
{
    Q_OBJECT
public:
    ToolWidget( const ToolList &tools,
                QWidget* parent = nullptr,
                Qt::WindowFlags f = Qt::WindowFlags() );

Q_SIGNALS:
    void toolSelected(ITool &tool);
    void toolDisabled();

private Q_SLOTS:
    void on_toolButtonClicked(bool checked);

private:
    ToolList m_toollist;
    QMap<QToolButton*, ITool*> m_buttonToolMap;
};

#endif // TOOLWIDGET_HPP
