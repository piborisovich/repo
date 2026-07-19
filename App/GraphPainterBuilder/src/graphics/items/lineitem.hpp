#ifndef LINEITEM_HPP
#define LINEITEM_HPP

#include "graphicsitem.hpp"
#include <QGraphicsLineItem>

/*!
 * \brief Линия
 */
class LineItem : public GraphicsItem<QGraphicsLineItem>
{
public:
    explicit LineItem(const QLineF &line, QGraphicsItem *parent = nullptr);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QPointF translatedVertex(const QPointF &cursorDxDy, int vertexIndex) const override;
    void setPoint(const QPointF &point, int vertexIndex) override;

    int findVertexPositionUnderCursor(const QPointF &pos, QPointF &point) const override;

    qreal strokeWidth() const override;

};

#endif // LINEITEM_HPP
