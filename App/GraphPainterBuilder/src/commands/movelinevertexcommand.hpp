#ifndef MOVELINEVERTEXCOMMAND_HPP
#define MOVELINEVERTEXCOMMAND_HPP

#include "movevertexcommand.hpp"
#include "lineitem.hpp"

#include <QGraphicsLineItem>
#include <QUndoCommand>

namespace Commands
{

/*!
 * \brief Команда перетаскивания вершины
 */
class MoveLineVertexCommand : public MoveVertexCommand
{
public:
    MoveLineVertexCommand(LineItem *lineItem,
                          int vertexIndex,
                          const QPointF& oldLocalPos,
                          const QPointF& newLocalPos,
                          QUndoCommand* parent = nullptr);

protected:
    virtual void updateVertexPosition(const QPointF& localPos) override;

private:
    LineItem *m_lineItem;
};

} //Commands

#endif // MOVELINEVERTEXCOMMAND_HPP
