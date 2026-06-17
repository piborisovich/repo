#include "movevertexcommand.hpp"

namespace Commands
{

MoveVertexCommand::MoveVertexCommand(QGraphicsPathItem *pathItem,
                                     int vertexIndex,
                                     const QPointF &oldLocalPos,
                                     const QPointF &newLocalPos,
                                     QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_pathItem(pathItem)
    , m_index(vertexIndex)
    , m_oldPos(oldLocalPos)
    , m_newPos(newLocalPos)
{
    setText("Move vertex");
}

void MoveVertexCommand::undo()
{
    updateVertexPosition(m_newPos);
}

void MoveVertexCommand::redo()
{
    updateVertexPosition(m_oldPos);
}

void MoveVertexCommand::updateVertexPosition(const QPointF& localPos) {

    if (!m_pathItem) return;

    QPainterPath path = m_pathItem->path();
    // Прямое изменение координат вершины по её индексу в локальной системе координат
    path.setElementPositionAt(m_index, localPos.x(), localPos.y());
    m_pathItem->setPath(path);
}

} //Commands
