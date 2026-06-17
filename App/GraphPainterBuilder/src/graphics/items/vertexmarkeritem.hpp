#ifndef VERTEXMARKERITEM_HPP
#define VERTEXMARKERITEM_HPP

#include <QGraphicsEllipseItem>
#include <QUndoStack>

class VertexMarkerItem : public QGraphicsEllipseItem
{
public:
    explicit VertexMarkerItem(QGraphicsItem *parent = nullptr);

    void deactivate();

    void setVertexPos(const QPointF& pos);

    int vertexIndex() const;
    void setVertexIndex(int newVertexIndex);

private:
    int m_vertexIndex;
};

#endif // VERTEXMARKERITEM_HPP
