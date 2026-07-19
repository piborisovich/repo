#include "selecttool.hpp"

#include <QGraphicsItem>


SelectTool::SelectTool(QObject *parent) :
    Tool("Select",
         std::make_shared<ToolSettings>(),
         ":/select.png",
         parent)
{
    settings()->setDragMode(QGraphicsView::RubberBandDrag);

    if ( settingsWidget() ) {
        settingsWidget()->setVisible(false);
    }
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

void SelectTool::select()
{
    if ( scene() ) {

        const auto allItems =  scene()->items();

        for ( QGraphicsItem *item : allItems ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
        }
    }
}
