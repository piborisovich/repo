#include "vertexmarkeritem.hpp"

#include <QPen>

static constexpr int VERTEX_WIDTH_PX = 10;

VertexMarkerItem::VertexMarkerItem(QGraphicsItem *parent)
    : QGraphicsEllipseItem(-VERTEX_WIDTH_PX / 2, -VERTEX_WIDTH_PX / 2,
                           VERTEX_WIDTH_PX, VERTEX_WIDTH_PX, parent)
    , m_vertexIndex(-1)
{
    setPen( QPen( Qt::red, 2 ) );
    setBrush(QBrush(Qt::transparent));
    setZValue(1000);

    setVisible(false);
}

void VertexMarkerItem::deactivate()
{
    setVisible(false);
}

void VertexMarkerItem::setVertexPos(const QPointF &pos)
{
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsMovable, false);

    setPos(pos);
    setVisible(true);
}

int VertexMarkerItem::vertexIndex() const
{
    return m_vertexIndex;
}

void VertexMarkerItem::setVertexIndex(int newVertexIndex)
{
    m_vertexIndex = newVertexIndex;
}
