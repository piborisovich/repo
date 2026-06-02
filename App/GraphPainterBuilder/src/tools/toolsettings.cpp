#include "toolsettings.hpp"

ToolSettings::ToolSettings()
    : m_brushSize(5)
    , m_dragMode(QGraphicsView::NoDrag)
    , m_isMovable(false)
    , m_isSelectable(false)
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

bool ToolSettings::movable() const
{
    return m_isMovable;
}

void ToolSettings::setMovable(bool newIsMovable)
{
    m_isMovable = newIsMovable;
}

bool ToolSettings::selectable() const
{
    return m_isSelectable;
}

void ToolSettings::setSelectable(bool newIsSelectable)
{
    m_isSelectable = newIsSelectable;
}
