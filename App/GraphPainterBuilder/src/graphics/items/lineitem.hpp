#ifndef LINEITEM_HPP
#define LINEITEM_HPP

#include "vertexmarkeritem.hpp"

#include <QGraphicsLineItem>

/*!
 * \brief Линия
 */
class LineItem : public QGraphicsLineItem
{
public:
    explicit LineItem(const QLineF &line, QGraphicsItem *parent = nullptr);

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    VertexMarkerItem* m_vertex;
};

#endif // LINEITEM_HPP
