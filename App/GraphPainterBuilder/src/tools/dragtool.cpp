#include "dragtool.hpp"


DragTool::DragTool(QObject *parent) :
    Tool("Drag",
         std::make_shared<ToolSettings>(),
         ":/drag.png",
         parent)
{
    settings()->setDragMode(QGraphicsView::ScrollHandDrag);

    if ( settingsWidget() ) {
        settingsWidget()->setVisible(false);
    }
}

const QCursor &DragTool::getCursor() const
{
    static QCursor cursor(Qt::ClosedHandCursor);

    return cursor;
}

void DragTool::handleMousePress(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
}

void DragTool::handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
}

void DragTool::handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos)
{
    Q_UNUSED(buttons);
    Q_UNUSED(scenePos);
}
