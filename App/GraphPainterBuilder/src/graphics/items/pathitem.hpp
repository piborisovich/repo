#ifndef PATHITEM_HPP
#define PATHITEM_HPP

#include "graphicsitem.hpp"

#include <QGraphicsPathItem>

class PathItem : public GraphicsItem<QGraphicsPathItem>
{
public:
    PathItem( const QPainterPath &painterPath, QGraphicsItem *parent = nullptr );

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    virtual QPointF translatedVertex(const QPointF &cursorDxDy, int vertexIndex) const override;
    virtual void setPoint(const QPointF &point, int vertexIndex) override;

    int findVertexPositionUnderCursor(const QPointF &pos, QPointF &point) const override;

private:

};

#endif // PATHITEM_HPP
