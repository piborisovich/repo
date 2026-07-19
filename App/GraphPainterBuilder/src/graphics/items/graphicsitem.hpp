#ifndef GRAPHICSITEM_HPP
#define GRAPHICSITEM_HPP

#include "graphicsscene.hpp"
#include "itempositionchangecommand.hpp"
#include "vertexmarkeritem.hpp"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

template<class BaseItem>
class GraphicsItem : public BaseItem
{
public:
    template<typename ...Args>
    explicit GraphicsItem(Args&&... args)
        : BaseItem(std::forward<Args>(args)...)
        , m_scene(nullptr)
        , m_vertex( new VertexMarkerItem(5, this) )
    {
        this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
        this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
        this->setAcceptHoverEvents(true); // Включаем отслеживание мыши без клика
    }

    GraphicsScene *graphicsScene() const
    {
        return m_scene;
    }

protected:

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override
    {
        if ( change == QGraphicsItem::ItemSceneHasChanged ) {
            m_scene = qvariant_cast<GraphicsScene*>(value);
        }

        //Ограничение перемещения
        if ( change == QGraphicsItem::ItemPositionChange && this->scene() )
        {
            QPointF newPos = value.toPointF();

            QRectF rect = this->boundingRect();
            QRectF sceneRect = this->scene()->sceneRect();

            // Вычисляем будущие границы элемента на сцене с учётом новой позиции
            qreal leftBound = newPos.x() + rect.left();
            qreal rightBound = newPos.x() + rect.right();
            qreal topBound = newPos.y() + rect.top();
            qreal bottomBound = newPos.y() + rect.bottom();

            // Корректируем X, если элемент выходит за левую или правую границу
            if (leftBound < sceneRect.left()) {
                newPos.setX(sceneRect.left() - rect.left());
            } else if (rightBound > sceneRect.right()) {
                newPos.setX(sceneRect.right() - rect.right());
            }

            // Корректируем Y, if элемент выходит за верхнюю или нижнюю границу
            if (topBound < sceneRect.top()) {
                newPos.setY(sceneRect.top() - rect.top());
            } else if (bottomBound > sceneRect.bottom()) {
                newPos.setY(sceneRect.bottom() - rect.bottom());
            }

            m_newPos = newPos;

            return newPos;
        }

        if ( change == QGraphicsItem::ItemSelectedHasChanged && value == false ) {
            m_vertex->deactivate();
        }

        return BaseItem::itemChange(change, value);
    }

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override
    {
        if ( this->flags() & QGraphicsItem::ItemIsSelectable ) {

            QPointF point;
            int bestIndex = findVertexPositionUnderCursor( event->pos(), point );

            // Активируем или скрываем маркер
            if (bestIndex == -1) {
                m_vertex->deactivate();
            } else {
                m_vertex->setVertexPos(point);
            }

            m_vertex->setVertexIndex(bestIndex); 
        }

        BaseItem::hoverMoveEvent(event);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override
    {
        m_vertex->deactivate();
        BaseItem::hoverLeaveEvent(event);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        if ( event->button() == Qt::LeftButton ) {
            m_prevPos = this->pos();
            m_newPos = m_prevPos;
        }
        BaseItem::mousePressEvent(event);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override
    {
        if ( event->buttons() & Qt::LeftButton ) {
            if ( m_vertex->vertexIndex() == -1 ) {
                BaseItem::mouseMoveEvent(event);
            } else {
                QPointF dxdy = event->pos() - event->lastPos();
                auto newPoint = translatedVertex( dxdy, m_vertex->vertexIndex() );
                if ( this->scene()->sceneRect().contains( this->mapToScene( newPoint ) ) ) {
                    setPoint(newPoint, m_vertex->vertexIndex());
                    m_vertex->setVertexPos(newPoint);
                }
            }

        } else {
            BaseItem::mouseMoveEvent(event);
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
    {
        if ( event->button() == Qt::LeftButton &&
            m_prevPos != m_newPos && m_scene ) {
            m_scene->addSceneCommand( new Commands::ItemPositionChangeCommand(this,
                                                                             m_prevPos,
                                                                             m_newPos) );
        }
        BaseItem::mouseReleaseEvent(event);
    }

    VertexMarkerItem* vertexMarker() const
    {
        return m_vertex;
    }

    virtual int findVertexPositionUnderCursor(const QPointF &pos, QPointF &point) const = 0;
    virtual QPointF translatedVertex(const QPointF &cursorDxDy, int vertexIndex) const = 0;
    virtual void setPoint(const QPointF &point, int vertexIndex) = 0;

private:
    GraphicsScene* m_scene;
    VertexMarkerItem* m_vertex; //!< Маркер вершины под курсором
    QPointF m_prevPos;
    QPointF m_newPos;
};

#endif // GRAPHICSITEM_HPP
