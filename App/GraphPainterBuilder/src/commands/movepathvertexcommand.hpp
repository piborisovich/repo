#ifndef MOVEPATHVERTEXCOMMAND_HPP
#define MOVEPATHVERTEXCOMMAND_HPP

#include "movevertexcommand.hpp"
#include "pathitem.hpp"

#include <QUndoCommand>

namespace Commands
{

/*!
 * \brief Команда перетаскивания вершины для пути
 */
class MovePathVertexCommand : public MoveVertexCommand
{
public:
    MovePathVertexCommand(PathItem *pathItem,
                          int vertexIndex,
                          const QPointF& oldLocalPos,
                          const QPointF& newLocalPos,
                          QUndoCommand* parent = nullptr);

protected:
    virtual void updateVertexPosition(const QPointF& localPos) override;

private:
    PathItem *m_pathItem;
};

} //Commands

#endif // MOVEPATHVERTEXCOMMAND_HPP
