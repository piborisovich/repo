#ifndef VERTEXMARKERITEM_HPP
#define VERTEXMARKERITEM_HPP

#include <QGraphicsEllipseItem>
#include <QUndoStack>

/*!
 * \brief Класс вершины
 */
class VertexMarkerItem : public QGraphicsEllipseItem
{
public:
    explicit VertexMarkerItem(QGraphicsItem *parent = nullptr);

    void deactivate();

    void setVertexPos(const QPointF& p);

    int vertexIndex() const;
    void setVertexIndex(int newVertexIndex);

    QPointF lastActivatingPos() const;

private:
    int m_vertexIndex;
    QPointF m_activatingPos;
};

#endif // VERTEXMARKERITEM_HPP
