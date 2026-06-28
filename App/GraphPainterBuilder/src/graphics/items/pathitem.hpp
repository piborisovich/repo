#ifndef PATHITEM_HPP
#define PATHITEM_HPP

#include "vertexmarkeritem.hpp"
#include <QGraphicsPathItem>

class PathItem : public QGraphicsPathItem
{
public:
    PathItem( const QPainterPath &painterPath, QGraphicsItem *parent = nullptr );

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    int findVertexPositionUnderCursor(const QPointF &pos, QPointF &point);

private:
    VertexMarkerItem* m_vertex;
};

#endif // PATHITEM_HPP
