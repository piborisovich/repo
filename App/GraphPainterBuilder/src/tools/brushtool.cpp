#include "brushtool.hpp"
#include "graphicsscene.hpp"

#include "macroaddcommand.hpp"

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
        m_brushItems.clear();
        processDrawing(scenePos);
    }
}

void BrushTool::handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);

    if ( m_brushItems.size() && scene() ) {
        // Регистрируем создание объекта в системе Undo/Redo
        scene()->addSceneCommand( new Commands::MacroAddCommand(scene(), m_brushItems));
    }
}

void BrushTool::handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos)
{
    if ( buttons & Qt::LeftButton ) {
        processDrawing(scenePos);
    }
}

void BrushTool::processDrawing(QPointF pos)
{
    auto scn = scene();

    if ( scn ) {

        QBrush brush(scn->currentColor());

        auto brushSize = settings()->brushSize();

        QPen pen(scn->currentColor(),
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

        newItem->setZValue(scn->currentLayerZ());

        scn->addItem(newItem);
        m_brushItems.push_back(newItem);
    }
}
