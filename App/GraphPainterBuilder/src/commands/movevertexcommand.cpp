#include "movevertexcommand.hpp"

namespace Commands
{

MoveVertexCommand::MoveVertexCommand(int vertexIndex,
                                     const QPointF &oldLocalPos,
                                     const QPointF &newLocalPos,
                                     QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_index(vertexIndex)
    , m_oldPos(oldLocalPos)
    , m_newPos(newLocalPos)
{
    setText("Move vertex");
}

void MoveVertexCommand::undo()
{
    updateVertexPosition(m_oldPos);
}

void MoveVertexCommand::redo()
{
    updateVertexPosition(m_newPos);
}

} //Commands
