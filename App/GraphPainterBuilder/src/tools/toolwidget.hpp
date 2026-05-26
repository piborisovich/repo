#ifndef TOOLWIDGET_HPP
#define TOOLWIDGET_HPP

#include "toollist.hpp"

#include <QWidget>

/*!
 * \brief Отображение инструментов
 */
class ToolWidget : public QWidget
{
public:
    ToolWidget( const ToolList &tools,
                QWidget* parent = nullptr,
               Qt::WindowFlags f = Qt::WindowFlags() );

private:
    ToolList m_toollist;
};

#endif // TOOLWIDGET_HPP
