#include "movelinevertexcommand.hpp"

namespace Commands
{

MoveLineVertexCommand::MoveLineVertexCommand(LineItem *lineItem,
                                             int vertexIndex,
                                             const QPointF &oldLocalPos,
                                             const QPointF &newLocalPos,
                                             QUndoCommand *parent)
    : MoveVertexCommand(vertexIndex, oldLocalPos, newLocalPos, parent)
    , m_lineItem(lineItem)
{
}

void MoveLineVertexCommand::updateVertexPosition(const QPointF& localPos) {

    if (!m_lineItem) return;

    QLineF line = m_lineItem->line();

    if ( m_index == 0 ) {
        line.setP1(localPos);
    } else {
        line.setP2(localPos);
    }

    m_lineItem->setLine(line);
   /* QPainterPath path = m_pathItem->path();
    // Прямое изменение координат вершины по её индексу в локальной системе координат
    path.setElementPositionAt(m_index, localPos.x(), localPos.y());
    m_pathItem->setPath(path);*/
}

} //Commands
