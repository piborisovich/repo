#include "itempositionchangecommand.hpp"

#include <QGraphicsItem>

namespace Commands
{

ItemPositionChangeCommand::ItemPositionChangeCommand(QGraphicsItem *item,
                                                     const QPointF& oldLocalPos,
                                                     const QPointF& newLocalPos,
                                                     QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_item(item)
    , m_oldPos(oldLocalPos)
    , m_newPos(newLocalPos)
{
}

void ItemPositionChangeCommand::undo()
{
    if ( m_item ) {
        m_item->setPos(m_oldPos);
    }
}

void ItemPositionChangeCommand::redo()
{
    if ( m_item ) {
        m_item->setPos(m_newPos);
    }
}

} //Commands