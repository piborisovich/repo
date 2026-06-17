#include "brushtool.hpp"
#include "graphicsscene.hpp"

#include "additemcommand.hpp""

BrushTool::BrushTool(QObject *parent) :
    Tool("Brush",
         std::make_shared<ToolSettings>(),
         ":/brush.png",
         parent)
{
}

void BrushTool::handleMousePress(Qt::MouseButton button, const QPointF &scenePos)
{
    if ( button == Qt::LeftButton ) {
        processDrawing(scenePos);
    }
}

void BrushTool::handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
}

void BrushTool::handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos)
{
    if ( buttons & Qt::LeftButton ) {
        processDrawing(scenePos);
    }
}

void BrushTool::processDrawing(QPointF pos)
{
    GraphicsScene *gScene = qobject_cast<GraphicsScene*>(scene());

    if ( gScene == nullptr ) return;

    QBrush brush(gScene->currentColor());

    auto brushSize = settings()->brushSize();

    QPen pen(gScene->currentColor(),
             brushSize,
             Qt::SolidLine,
             Qt::RoundCap,
             Qt::RoundJoin);

    QGraphicsItem *newItem = new QGraphicsEllipseItem(pos.x() - brushSize/2.0,
                                                      pos.y() - brushSize/2.0,
                                                      brushSize,
                                                      brushSize);

    static_cast<QGraphicsEllipseItem*>(newItem)->setPen(pen);
    static_cast<QGraphicsEllipseItem*>(newItem)->setBrush(brush);

    newItem->setZValue(gScene->currentLayerZ());
    // Регистрируем создание объекта в системе Undo/Redo
    gScene->addSceneCommand( new Commands::AddItemCommand(scene(), newItem));
}
