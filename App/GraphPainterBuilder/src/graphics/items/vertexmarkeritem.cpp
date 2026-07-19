#include "vertexmarkeritem.hpp"

#include <QPen>

VertexMarkerItem::VertexMarkerItem(int vertexWith, QGraphicsItem *parent)
    : QGraphicsEllipseItem(-vertexWith / 2, -vertexWith / 2,
                           vertexWith, vertexWith, parent)
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

void VertexMarkerItem::setVertexPos(const QPointF &p)
{
    if ( !isVisible() ) {
        m_activatingPos = p;
    }
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsMovable, false);

    setPos(p);
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

QPointF VertexMarkerItem::lastActivatingPos() const
{
    return m_activatingPos;
}
