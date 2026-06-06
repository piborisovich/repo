#include "graphicsscene.hpp"
#include "commands.hpp"
#include "itool.hpp"

#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>


const QBrush GraphicsScene::BACKGROUND_BRUSH = QBrush(QColor(255, 255, 255));

GraphicsScene::GraphicsScene(std::shared_ptr<QUndoStack> undoStack, QObject *parent) : QGraphicsScene(parent)
    , m_currentLayerZ(20)
    , m_currentColor(Qt::black)
    , m_currentTool(nullptr)
    , m_undoStack(undoStack)
{
    setBackgroundBrush(BACKGROUND_BRUSH);
}

void GraphicsScene::addSceneListener(ISceneListener *listener)
{
    if ( listener && !m_listeners.contains(listener) ) {
        m_listeners.push_back(listener);
    }
}

void GraphicsScene::removeSceneListener(ISceneListener *listener)
{
    if ( listener ) {
        m_listeners.removeOne(listener);
    }
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    auto pos = event->scenePos();
    foreach (auto *listener, m_listeners) {
        listener->mousePressed( event->button(), pos );
    }

    if ( m_currentTool ) {
        m_currentTool->handleMousePress( event->button(), pos );
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    auto pos = event->scenePos();

    foreach (auto *listener, m_listeners) {
        listener->mouseMoved( pos );
    }

    if ( m_currentTool ) {
        m_currentTool->handleMouseMove( event->buttons(), pos );
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    auto pos = event->scenePos();

    foreach (auto *listener, m_listeners) {
        listener->mouseReleased( event->button(), pos );
    }

    if ( m_currentTool ) {
        m_currentTool->handleMouseRelease( event->button(), pos );
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

QColor GraphicsScene::currentColor() const
{
    return m_currentColor;
}

void GraphicsScene::setCurrentColor(const QColor &newCurrentColor)
{
    m_currentColor = newCurrentColor;
}

void GraphicsScene::addSceneCommand(QUndoCommand *command)
{
    if ( m_undoStack && command ) {
        m_undoStack->push( command );
    }
}

void GraphicsScene::changeCurrentTool(ITool* tool)
{
    if ( tool != m_currentTool ) {
        m_currentTool = tool;

        for ( auto* view : views() ) {
            if ( tool && tool->settings() ) {
                auto sts = tool->settings();
                view->setCursor(tool->getCursor());
                view->setDragMode( sts->dragMode() );
            } else {
                view->setCursor( QCursor() );
                view->setDragMode( QGraphicsView::NoDrag );
            }
        }

        auto allItems = items();

        for (QGraphicsItem *item : allItems) {
            if ( tool && tool->settings() ) {
                auto sts = tool->settings();
                item->setFlag(QGraphicsItem::ItemIsSelectable, sts->selectable());
                item->setFlag(QGraphicsItem::ItemIsMovable, sts->movable());
            } else {
                item->setFlag(QGraphicsItem::ItemIsSelectable, false);
                item->setFlag(QGraphicsItem::ItemIsMovable, false);
            }

            if ( (item->flags() & QGraphicsItem::ItemIsSelectable) == 0 ) {
                item->setSelected(false);
            }
        }
    }
}

void GraphicsScene::addImage(const QString & path, QSize &imageSize)
{
    QGraphicsItem *newItem = nullptr;

    QPixmap pixmap(path);

    if ( !pixmap.isNull() ) {
        newItem = new QGraphicsPixmapItem(pixmap);
    }

    if ( newItem ) {
        newItem->setZValue(m_currentLayerZ);
        imageSize = pixmap.size();
        //newItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        m_undoStack->push(new Commands::AddItemCommand(this, newItem));
    }

}

int GraphicsScene::currentLayerZ() const
{
    return m_currentLayerZ;
}

void GraphicsScene::setCurrentLayerZ(int newCurrentLayerZ)
{
    m_currentLayerZ = newCurrentLayerZ;
}
