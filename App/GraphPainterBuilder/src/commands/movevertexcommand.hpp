#ifndef MOVEVERTEXCOMMAND_HPP
#define MOVEVERTEXCOMMAND_HPP

#include <QPointF>
#include <QUndoCommand>

namespace Commands
{

/*!
 * \brief Команда перетаскивания вершины
 */
class MoveVertexCommand : public QUndoCommand
{
public:
    MoveVertexCommand(int vertexIndex,
                      const QPointF& oldLocalPos,
                      const QPointF& newLocalPos,
                      QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

protected:
    virtual void updateVertexPosition(const QPointF& localPos) = 0;

protected:
    int m_index;
    QPointF m_oldPos;
    QPointF m_newPos;
};

} //Commands

#endif // MOVEVERTEXCOMMAND_HPP
