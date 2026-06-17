#ifndef MOVEVERTEXCOMMAND_HPP
#define MOVEVERTEXCOMMAND_HPP

#include <QUndoCommand>
#include <QGraphicsPathItem>

namespace Commands
{

/*!
 * \brief Команда перетаскивания вершины
 */
class MoveVertexCommand : public QUndoCommand
{
public:
    MoveVertexCommand(QGraphicsPathItem* pathItem,
                      int vertexIndex,
                      const QPointF& oldLocalPos,
                      const QPointF& newLocalPos,
                      QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    void updateVertexPosition(const QPointF& localPos);

private:
    QGraphicsPathItem* m_pathItem;
    int m_index;
    QPointF m_oldPos;
    QPointF m_newPos;
};

} //Commands

#endif // MOVEVERTEXCOMMAND_HPP
