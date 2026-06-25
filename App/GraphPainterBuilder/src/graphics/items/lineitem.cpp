#include "lineitem.hpp"
#include "graphicsscene.hpp"
#include "movelinevertexcommand.hpp"

#include <QGraphicsSceneHoverEvent>

LineItem::LineItem(const QLineF &line,
                   QGraphicsItem *parent)
    : QGraphicsLineItem(line, parent )
{
    m_vertex = new VertexMarkerItem(this);

    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true); // Включаем отслеживание мыши без клика
}

void LineItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if ( flags() & ItemIsSelectable ) {

        QPointF pos = event->pos();

        QLineF l = line();
        QPointF p1 = l.p1();
        QPointF p2 = l.p2();

        double dx = p1.x() - pos.x();
        double dy = p1.y() - pos.y();
        double distSq = dx * dx + dy * dy;

        int bestIndex = -1;

        double minDistanceSq = 100.0; // Максимальный радиус захвата в квадрате (10 пикселей)

        if (distSq < minDistanceSq) {
            minDistanceSq = distSq;
            bestIndex = 0;
        }

        dx = p2.x() - pos.x();
        dy = p2.y() - pos.y();
        distSq = dx * dx + dy * dy;

        if (distSq < minDistanceSq) {
            bestIndex = 1;
        }

        // Активируем или скрываем маркер
        if (bestIndex != -1) {
            m_vertex->setVertexPos(bestIndex == 0 ? l.p1() :l. p2());
        } else {
            m_vertex->deactivate();
        }

        m_vertex->setVertexIndex(bestIndex);

        QGraphicsLineItem::hoverMoveEvent(event);
    }
}

void LineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_vertex->deactivate();
    QGraphicsLineItem::hoverLeaveEvent(event);
}

void LineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->buttons() & Qt::LeftButton ) {

        QLineF l = line();
        QPointF dxdy = event->pos() - event->lastPos();

        if ( m_vertex->vertexIndex() == 0 ) {
            l.setP1(l.p1() + dxdy);
            m_vertex->setVertexPos(l.p1());
            setLine(l);
            return;
        } else if ( m_vertex->vertexIndex() == 1 ) {
            l.setP2(l.p2() + dxdy);
            m_vertex->setVertexPos(l.p2());
            setLine(l);
            return;
        }
    }

    QGraphicsLineItem::mouseMoveEvent(event);
}

void LineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    GraphicsScene* gScene = dynamic_cast<GraphicsScene*>(scene());

    if ( gScene && m_vertex->isVisible() ) {
        gScene->addSceneCommand( new Commands::MoveLineVertexCommand(this,
                                                                    m_vertex->vertexIndex(),
                                                                    m_vertex->lastActivatingPos(),
                                                                    m_vertex->vertexIndex() == 0 ? line().p1() : line().p2()) );
    }

    QGraphicsLineItem::mouseReleaseEvent(event);
}

QVariant LineItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if ( change == ItemSelectedHasChanged && value == false ) {
        m_vertex->deactivate();
    } else if ( change == ItemCursorHasChanged ) {
        qDebug() << change;
    }
    return QGraphicsLineItem::itemChange(change, value);
}
