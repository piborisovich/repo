#include "selecttool.hpp"


SelectTool::SelectTool(QObject *parent) :
    Tool("Select",
         std::make_shared<ToolSettings>(),
         ":/select.png",
         parent)
{
    settings()->setDragMode(QGraphicsView::RubberBandDrag);
    settings()->setMovable(true);
    settings()->setSelectable(true);
}

void SelectTool::handleMousePress(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
}

void SelectTool::handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
}

void SelectTool::handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos)
{
    Q_UNUSED(buttons);
    Q_UNUSED(scenePos);
}
