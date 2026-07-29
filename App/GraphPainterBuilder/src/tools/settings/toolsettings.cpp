#include "toolsettings.hpp"
#include "constants.hpp"

ToolSettings::ToolSettings()
    : m_penSize(Constants::DEFAULT_PEN_SIZE)
    , m_brushSize(Constants::DEFAULT_BRUSH_SIZE)
    , m_dragMode(QGraphicsView::NoDrag)
{
}

qreal ToolSettings::penSize() const
{
    return m_penSize;
}

void ToolSettings::setPenSize(qreal newPenSize)
{
    m_penSize = newPenSize;
}

QGraphicsView::DragMode ToolSettings::dragMode() const
{
    return m_dragMode;
}

void ToolSettings::setDragMode(const QGraphicsView::DragMode &newDragMode)
{
    m_dragMode = newDragMode;
}

qreal ToolSettings::brushSize() const
{
    return m_brushSize;
}

void ToolSettings::setBrushSize(qreal newBrushSize)
{
    m_brushSize = newBrushSize;
}
