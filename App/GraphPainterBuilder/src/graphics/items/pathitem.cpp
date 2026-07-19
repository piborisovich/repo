#include "pathitem.hpp"
#include "graphicsscene.hpp"
#include "movepathvertexcommand.hpp"

#include <QGraphicsSceneHoverEvent>

PathItem::PathItem(const QPainterPath &painterPath,
                   QGraphicsItem *parent)
    : GraphicsItem<QGraphicsPathItem>(painterPath, parent)
{
}

void PathItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    GraphicsScene* gScene = dynamic_cast<GraphicsScene*>(scene());

    if ( gScene && vertexMarker()->isVisible() ) {
        gScene->addSceneCommand( new Commands::MovePathVertexCommand(this,
                                                                    vertexMarker()->vertexIndex(),
                                                                    vertexMarker()->lastActivatingPos(),
                                                                    path().elementAt(vertexMarker()->vertexIndex())) );
    }

    GraphicsItem<QGraphicsPathItem>::mouseReleaseEvent(event);
}

QPointF PathItem::translatedVertex(const QPointF &cursorDxDy, int vertexIndex) const
{
    QPainterPath p = path();
    auto elem = p.elementAt(vertexIndex);

    return QPointF(elem) + QPointF( 0, cursorDxDy.y() );
}

void PathItem::setPoint(const QPointF &point, int vertexIndex)
{
    QPainterPath p = path();
    p.setElementPositionAt( vertexIndex, point.x(), point.y() );
    setPath(p);
}

int PathItem::findVertexPositionUnderCursor(const QPointF &pos, QPointF &point) const
{
    int bestIndex = -1;
    QPainterPath p = path();

    double minDistanceSq = 25.0; // Максимальный радиус захвата в квадрате (5 пикселей)

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

qreal PathItem::strokeWidth() const
{
    return pen().widthF();
}
