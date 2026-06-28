#include "movepathvertexcommand.hpp"

namespace Commands
{

MovePathVertexCommand::MovePathVertexCommand(PathItem *pathItem,
                                             int vertexIndex,
                                             const QPointF& oldLocalPos,
                                             const QPointF& newLocalPos,
                                             QUndoCommand* parent)
    : MoveVertexCommand(vertexIndex, oldLocalPos, newLocalPos, parent)
    , m_pathItem(pathItem)
{
}

void MovePathVertexCommand::updateVertexPosition(const QPointF &localPos)
{
    QPainterPath path = m_pathItem->path();
    // Прямое изменение координат вершины по её индексу в локальной системе координат
    path.setElementPositionAt(m_index, localPos.x(), localPos.y());
    m_pathItem->setPath(path);
}

} //Commands
