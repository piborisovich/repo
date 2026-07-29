#include "graphicsscene.hpp"
#include "additemcommand.hpp"
#include "macrodeletecommand.hpp"
#include "graphicsitem.hpp"
#include "itool.hpp"

#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QUndoStack>

using PixmapItem = GraphicsItem<QGraphicsPixmapItem>;

const QBrush GraphicsScene::BACKGROUND_BRUSH = QBrush(Qt::white);
const QRectF GraphicsScene::DEFAULT_SCENE_RECT = QRectF(0, 0, 800, 600);

GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene(parent)
    , m_currentLayerZ(20)
    , m_currentColor(Qt::black)
    , m_currentTool(nullptr)
    , m_undoStack(new QUndoStack(this))
{
    setSceneRect(DEFAULT_SCENE_RECT);
    setBackgroundBrush(BACKGROUND_BRUSH);

    auto result = connect(m_undoStack,
                          &QUndoStack::canUndoChanged,
                          this,
                          &GraphicsScene::canUndoChanged);
    Q_ASSERT(result);

    result = connect(m_undoStack,
                     &QUndoStack::canRedoChanged,
                     this,
                     &GraphicsScene::canRedoChanged);
    Q_ASSERT(result);

    result = connect(this,
                     &GraphicsScene::selectionChanged,
                     this,
                     &GraphicsScene::on_selectionChanged);
    Q_ASSERT(result);
}

GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::undo()
{
    m_undoStack->undo();
}

void GraphicsScene::redo()
{
    m_undoStack->redo();
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

    if ( sceneRect().contains(pos) ) {

        foreach (auto *listener, m_listeners) {
            listener->mousePressed( event->button(), pos );
        }

        if ( m_currentTool ) {
            m_currentTool->handleMousePress( event->button(), pos );
        }
    }

    //Отработка нажатия на правую кнопку мыши
    if ( event->button() == Qt::RightButton ) {
        //Выделить самый верхний элемент под курсором
        auto item = itemAt(pos, QTransform());

        if ( item && !item->isSelected() ) {
            clearSelection();
            item->setSelected(true);
        } else if ( item == nullptr ) {
            event->accept();
            return;
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    auto pos = event->scenePos();

    if ( sceneRect().contains(pos) ) {

        foreach (auto *listener, m_listeners) {
            listener->mouseMoved( pos );
        }

        if ( m_currentTool ) {
            m_currentTool->handleMouseMove( event->buttons(), pos );
        }
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

void GraphicsScene::on_selectionChanged()
{
    auto items = selectedItems();

    if ( items.size() == 1 ) {
        Q_EMIT itemSelected(items.first());
    } else {
        Q_EMIT itemDeselected();
    }
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

void GraphicsScene::clearScene()
{
    if ( m_currentTool ) {
        m_currentTool->unselect();
    }
    clear();
    m_undoStack->clear();

    if ( m_currentTool ) {
        m_currentTool->select();
    }
}

void GraphicsScene::changeCurrentTool(ITool* tool)
{
    if ( tool != m_currentTool ) {
        m_currentTool = tool;

        for ( auto* view : views() ) {
            if ( tool ) {
                view->setCursor( tool->getCursor() );
                if ( tool->settings() ) {
                    auto sts = tool->settings();
                    view->setDragMode( sts->dragMode() );
                } else {
                    view->setDragMode( QGraphicsView::NoDrag );
                }
            } else {
                view->setCursor( QCursor() );
                view->setDragMode( QGraphicsView::NoDrag );
            }
        }

        if ( tool == nullptr ) {
            foreach (auto &item, items() ) {
                item->setSelected(false);
                item->setFlag(QGraphicsItem::ItemIsMovable, false);
                item->setFlag(QGraphicsItem::ItemIsSelectable, false);
            }
        }
    }
}

void GraphicsScene::addImage(const QString & path, QSize &imageSize)
{
    QGraphicsItem *newItem = nullptr;

    QPixmap pixmap(path);

    if ( !pixmap.isNull() ) {
        newItem = new PixmapItem(pixmap);
    }

    if ( newItem ) {
        if ( m_currentTool ) {
            m_currentTool->unselect();
        }

        QRectF sr = sceneRect();

        newItem->setZValue(m_currentLayerZ);
        imageSize = pixmap.size();
        //newItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        m_undoStack->push(new Commands::AddItemCommand(this, newItem));

        setSceneRect( QRectF( QPointF(0, 0),
                            QSizeF( qMax( sr.width(), qreal(imageSize.width()) ),
                                    qMax( sr.height(), qreal(imageSize.height()) ) ) ) );

        if ( m_currentTool ) {
            m_currentTool->select();
        }
    }

}

[[clang::suppress]]
void GraphicsScene::removeItems(const QList<QGraphicsItem *> &items)
{
    addSceneCommand( new Commands::MacroDeleteCommand(this, items) );
}

int GraphicsScene::currentLayerZ() const
{
    return m_currentLayerZ;
}

void GraphicsScene::setCurrentLayerZ(int newCurrentLayerZ)
{
    m_currentLayerZ = newCurrentLayerZ;
}

void GraphicsScene::clearLayer(int layerZ)
{
    if ( m_currentTool ) {
        m_currentTool->unselect();
    }

    const QList<QGraphicsItem*> allItems = items();

    for (QGraphicsItem *item : allItems) {
        if (item->zValue() == layerZ) {
            removeItem(item);
            delete item;
        }
    }

    m_undoStack->clear(); // Очищаем историю, так как элементы физически удалены

    if ( m_currentTool ) {
        m_currentTool->select();
    }
}
