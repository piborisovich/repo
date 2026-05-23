#include "graphicsscene.hpp"
#include "commands.hpp"

#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>


const QBrush GraphicsScene::BACKGROUND_BRUSH = QBrush(QColor(255, 0, 0, 50));

GraphicsScene::GraphicsScene(QUndoStack *undoStack, QObject *parent) : QGraphicsScene(parent)
    , m_currentLayerZ(20)
    , m_brushSize(5)
    , m_currentColor(Qt::black)
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

    if ( event->button() == Qt::LeftButton ) {
        m_startPoint = pos;
        m_erasedItemsThisStroke.clear();

        if (m_currentTool == "Line") {

            QPen pen(m_currentColor,
                     m_brushSize,
                     Qt::SolidLine,
                     Qt::RoundCap,
                     Qt::RoundJoin);

            m_previewLine = new QGraphicsLineItem(QLineF(m_startPoint, m_startPoint));
            m_previewLine->setPen(pen);
            m_previewLine->setZValue(m_currentLayerZ);
            addItem(m_previewLine); // Временно добавляем для предпросмотра
        } else {
            processDrawing(event->scenePos());
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    foreach (auto *listener, m_listeners) {
        listener->mouseMoved( event->scenePos() );
    }

    if ( event->buttons() & Qt::LeftButton ) {
        if ( m_currentTool == "Line" && m_previewLine ) {
            m_previewLine->setLine( QLineF (m_startPoint, event->scenePos() ) );
        } else if (m_currentTool == "Brush" || m_currentTool == "Eraser") {
            processDrawing(event->scenePos());
        }
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    foreach (auto *listener, m_listeners) {
        listener->mouseReleased( event->button(), event->scenePos() );
    }

    if ( event->button() == Qt::LeftButton ) {
        if (m_currentTool == "Line" && m_previewLine) {
            // Передаем созданную линию под управление UndoStack
            removeItem(m_previewLine);
            m_undoStack->push( new Commands::AddItemCommand( this, m_previewLine) );
            m_previewLine = nullptr;
        }
        else if (m_currentTool == "Eraser" && !m_erasedItemsThisStroke.isEmpty()) {
            // Записываем все удаленные за один проход элементы в одну Undo-команду
            m_undoStack->push(new Commands::MacroDeleteCommand(this, m_erasedItemsThisStroke));
            m_erasedItemsThisStroke.clear();
        }
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

void GraphicsScene::processDrawing(QPointF pos)
{
    if (m_currentTool == "Eraser") {
        QList<QGraphicsItem*> itemsAtPos = items( QRectF( pos.x() - m_brushSize/2.0,
                                                 pos.y() - m_brushSize/2.0,
                                                 m_brushSize,
                                                 m_brushSize ) );

        for (QGraphicsItem *item : itemsAtPos) {
            if ( item->zValue() == m_currentLayerZ && !m_erasedItemsThisStroke.contains(item) && item != m_previewLine) {
                removeItem(item); // Убираем со сцены визуально
                m_erasedItemsThisStroke.append(item); // Запоминаем для Undo
            }
        }
        return;
    }

    QPen pen(m_currentColor,
             m_brushSize,
             Qt::SolidLine,
             Qt::RoundCap,
             Qt::RoundJoin);

    QBrush brush(m_currentColor);

    QGraphicsItem *newItem = nullptr;

    if ( m_currentTool == "Brush") {
        newItem = new QGraphicsEllipseItem(pos.x() - m_brushSize/2.0,
                                           pos.y() - m_brushSize/2.0,
                                           m_brushSize,
                                           m_brushSize);

        static_cast<QGraphicsEllipseItem*>(newItem)->setPen(Qt::NoPen);
        static_cast<QGraphicsEllipseItem*>(newItem)->setBrush(brush);
    }
    else if (m_currentTool == "Rectangle") {
        newItem = new QGraphicsRectItem(pos.x() - 25, pos.y() - 25, 50, 50);
        static_cast<QGraphicsRectItem*>(newItem)->setPen(pen);
    }
    else if (m_currentTool == "Circle") {
        newItem = new QGraphicsEllipseItem(pos.x() - 25, pos.y() - 25, 50, 50);
        static_cast<QGraphicsEllipseItem*>(newItem)->setPen(pen);
    }

    if (newItem) {
        newItem->setZValue(m_currentLayerZ);
        // Регистрируем создание объекта в системе Undo/Redo
        m_undoStack->push(new Commands::AddItemCommand(this, newItem));
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

int GraphicsScene::brushSize() const
{
    return m_brushSize;
}

void GraphicsScene::setBrushSize(int newBrushSize)
{
    m_brushSize = newBrushSize;
}

QString GraphicsScene::currentTool() const
{
    return m_currentTool;
}

void GraphicsScene::setCurrentTool(const QString &newCurrentTool)
{
    m_currentTool = newCurrentTool;
}

int GraphicsScene::currentLayerZ() const
{
    return m_currentLayerZ;
}

void GraphicsScene::setCurrentLayerZ(int newCurrentLayerZ)
{
    m_currentLayerZ = newCurrentLayerZ;
}
