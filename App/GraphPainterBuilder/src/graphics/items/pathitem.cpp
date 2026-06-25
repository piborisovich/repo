#include "pathitem.hpp"
#include "graphicsscene.hpp"
#include "movepathvertexcommand.hpp"

#include <QGraphicsSceneHoverEvent>

PathItem::PathItem(const QPainterPath &painterPath,
                   QGraphicsItem *parent)
    : QGraphicsPathItem(painterPath, parent)
{
    m_vertex = new VertexMarkerItem(this);

    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true); // Включаем отслеживание мыши без клика
}

void PathItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if ( flags() & ItemIsSelectable ) {

        QPointF point;
        int bestIndex = findVertexPositionUnderCursor( event->pos(), point );

        // Активируем или скрываем маркер
        if (bestIndex != -1) {
            m_vertex->setVertexPos(point);
        } else {
            m_vertex->deactivate();
        }

        m_vertex->setVertexIndex(bestIndex);

        QGraphicsPathItem::hoverMoveEvent(event);
    }
}

void PathItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_vertex->deactivate();
    QGraphicsPathItem::hoverLeaveEvent(event);
}

void PathItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->buttons() & Qt::LeftButton && m_vertex->vertexIndex() != -1 ) {

        QPainterPath p = path();
        auto elem = p.elementAt( m_vertex->vertexIndex() );
        QPointF dxdy( 0, event->pos().y() - event->lastPos().y() );

        QPointF newpoint = QPointF(elem) + dxdy;

        m_vertex->setVertexPos(newpoint);

        p.setElementPositionAt(m_vertex->vertexIndex(), newpoint.x(), newpoint.y());
        setPath(p);

        return;
    }

    QGraphicsPathItem::mouseMoveEvent(event);
}

void PathItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    GraphicsScene* gScene = dynamic_cast<GraphicsScene*>(scene());

    if ( gScene && m_vertex->isVisible() ) {
        gScene->addSceneCommand( new Commands::MovePathVertexCommand(this,
                                                                    m_vertex->vertexIndex(),
                                                                    m_vertex->lastActivatingPos(),
                                                                    path().elementAt(m_vertex->vertexIndex())) );
    }

    QGraphicsPathItem::mouseReleaseEvent(event);
}

QVariant PathItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if ( change == ItemSelectedHasChanged && value == false ) {
        m_vertex->deactivate();
    } else if ( change == ItemCursorHasChanged ) {
        qDebug() << change;
    }
    return QGraphicsPathItem::itemChange(change, value);
}

int PathItem::findVertexPositionUnderCursor(const QPointF &pos, QPointF &point)
{
    int bestIndex = -1;
    QPainterPath p = path();

    double minDistanceSq = 100.0; // Максимальный радиус захвата в квадрате (10 пикселей)

    for ( int i = 0; i < p.elementCount(); ++i ) {
        QPointF pt = QPointF(p.elementAt(i));
        double dx = pt.x() - pos.x();
        double dy = pt.y() - pos.y();
        double distSq = dx * dx + dy * dy;

        if (distSq < minDistanceSq) {
            minDistanceSq = distSq;
            bestIndex = i;
            point = pt;
        }
    }

    return bestIndex;
}
