#include "toolsettings.hpp"

ToolSettings::ToolSettings()
    : m_brushSize(5.0)
    , m_dragMode(QGraphicsView::NoDrag)
{
}

qreal ToolSettings::brushSize() const
{
    return m_brushSize;
}

void ToolSettings::setBrushSize(qreal newBrushSize)
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
