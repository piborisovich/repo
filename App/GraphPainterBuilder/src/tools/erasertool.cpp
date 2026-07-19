#include "erasertool.hpp"
#include "graphicsscene.hpp"
#include "macrodeletecommand.hpp"
#include "qforeach.h"

#include <QRectF>

EraserTool::EraserTool(QObject *parent) :
    Tool("Eraser",
         std::make_shared<ToolSettings>(),
         ":/eraser.png",
         parent)
{
}

void EraserTool::handleMousePress(Qt::MouseButton button, const QPointF &scenePos)
{
    if ( button == Qt::LeftButton ) {
        m_erasedItems.clear();
        processDrawing(scenePos);
    }
}

void EraserTool::handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(scenePos);

    if ( button == Qt::LeftButton ) {
        if ( !m_erasedItems.isEmpty() ) {
            GraphicsScene *gScene = scene();

            if ( gScene == nullptr ) return;
            // Записываем все удаленные за один проход элементы в одну Undo-команду
            gScene->addSceneCommand( new Commands::MacroDeleteCommand(scene(), m_erasedItems) );
            m_erasedItems.clear();
        }
    }
}

void EraserTool::handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos)
{
    if ( buttons & Qt::LeftButton ) {
        processDrawing(scenePos);
    }
}

void EraserTool::processDrawing(QPointF pos)
{
    GraphicsScene *scn = scene();

    if ( scn ) {

        auto brushSize = settings()->brushSize();

        QList<QGraphicsItem*> itemsAtPos = scene()->items( QRectF( pos.x() - brushSize/2.0,
                                                                  pos.y() - brushSize/2.0,
                                                                  brushSize,
                                                                  brushSize ) );
        foreach(QGraphicsItem *item, itemsAtPos) {
            if ( item->zValue() == scn->currentLayerZ() &&
                !m_erasedItems.contains(item) ) {
                scn->removeItem(item); // Убираем со сцены визуально
                m_erasedItems.append(item); // Запоминаем для Undo
            }
        }

    }

}
