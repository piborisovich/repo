#include "toolsettings.hpp"

ToolSettings::ToolSettings()
    : m_brushSize(5)
    , m_dragMode(QGraphicsView::NoDrag)
{
}

int ToolSettings::brushSize() const
{
    return m_brushSize;
}

void ToolSettings::setBrushSize(int newBrushSize)
{
    m_brushSize = newBrushSize;
}

QGraphicsView::DragMode ToolSettings::dragMode() const
{
    return m_dragMode;
}

void ToolSettings::setDragMode(const QGraphicsView::DragMode &newDragMode)
{
    m_dragMode = newDragMode;
}
