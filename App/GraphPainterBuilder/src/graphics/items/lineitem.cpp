#include "lineitem.hpp"
#include "graphicsscene.hpp"
#include "movelinevertexcommand.hpp"

#include <QGraphicsSceneHoverEvent>

LineItem::LineItem(const QLineF &line,
                   QGraphicsItem *parent)
    : GraphicsItem<QGraphicsLineItem>(line, parent)
{
}

void LineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton ) {

        GraphicsScene* gScene = graphicsScene();

        if ( gScene && vertexMarker()->isVisible() ) {
            gScene->addSceneCommand( new Commands::MoveLineVertexCommand(this,
                                                                        vertexMarker()->vertexIndex(),
                                                                        vertexMarker()->lastActivatingPos(),
                                                                        vertexMarker()->vertexIndex() == 0 ? line().p1() : line().p2()) );
        }

    }

    GraphicsItem<QGraphicsLineItem>::mouseReleaseEvent(event);
}

QPointF LineItem::translatedVertex(const QPointF &cursorDxDy, int vertexIndex) const
{
    if ( vertexIndex == 0 ) {
        return line().p1() + cursorDxDy;
    } else if ( vertexMarker()->vertexIndex() == 1 ) {
        return line().p2() + cursorDxDy;
    }

    return QPointF();
}

void LineItem::setPoint(const QPointF &point, int vertexIndex)
{
    QLineF l = line();

    if ( vertexIndex == 0 ) {
        l.setP1(point);
    } else if ( vertexIndex == 1 ) {
        l.setP2(point);
    }

    setLine(l);
}

int LineItem::findVertexPositionUnderCursor(const QPointF &pos, QPointF &point) const
{
    QLineF l = line();
    QPointF p1 = l.p1();
    QPointF p2 = l.p2();

    double dx = p1.x() - pos.x();
    double dy = p1.y() - pos.y();
    double distSq = dx * dx + dy * dy;

    int bestIndex = -1;

    double minDistanceSq = 25.0; // Максимальный радиус захвата в квадрате (5 пикселей)

    if (distSq < minDistanceSq) {
        minDistanceSq = distSq;
        bestIndex = 0;
        point = p1;
    }

    dx = p2.x() - pos.x();
    dy = p2.y() - pos.y();
    distSq = dx * dx + dy * dy;

    if (distSq < minDistanceSq) {
        bestIndex = 1;
        point = p2;
    }

    return bestIndex;
}

qreal LineItem::strokeWidth() const
{
    return pen().widthF();
}
