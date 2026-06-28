#include "circletool.hpp"

#include "graphicsscene.hpp"
#include "additemcommand.hpp"

CircleTool::CircleTool(QObject *parent) :
    Tool("Circle",
         std::make_shared<ToolSettings>(),
         ":/circle.png",
         parent)
{

}

void CircleTool::handleMousePress(Qt::MouseButton button, const QPointF &scenePos)
{
    if ( button == Qt::LeftButton ) {
        processDrawing(scenePos);
    }
}

void CircleTool::handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
}

void CircleTool::handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos)
{
    Q_UNUSED(buttons);
    Q_UNUSED(scenePos);
}

void CircleTool::processDrawing(QPointF pos)
{
    GraphicsScene *scn = scene();

    if ( scn ) {

        auto brushSize = settings()->brushSize();

        QPen pen(scn->currentColor(),
                 brushSize,
                 Qt::SolidLine,
                 Qt::RoundCap,
                 Qt::RoundJoin);

        QGraphicsItem *newItem = new QGraphicsEllipseItem(pos.x() - 25, pos.y() - 25, 50, 50);
        static_cast<QGraphicsEllipseItem*>(newItem)->setPen(pen);

        newItem->setZValue(scn->currentLayerZ());
        // Регистрируем создание объекта в системе Undo/Redo
        scn->addSceneCommand( new Commands::AddItemCommand(scene(), newItem));
    }
}
