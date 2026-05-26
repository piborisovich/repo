#include "toolwidget.hpp"

ToolWidget::ToolWidget(const ToolList &tools,
                       QWidget *parent,
                       Qt::WindowFlags f) : QWidget(parent, f)
    , m_toollist (tools)
{
}
