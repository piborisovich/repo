#ifndef ITEMPOSITIONCHANGECOMMAND_HPP
#define ITEMPOSITIONCHANGECOMMAND_HPP

#include <QUndoCommand>
#include <QGraphicsScene>

namespace Commands
{

/*!
 * \brief Команда изменения положения графического элемента
 */
class ItemPositionChangeCommand : public QUndoCommand
{
public:
    ItemPositionChangeCommand(QGraphicsItem *item,
                              const QPointF& oldLocalPos,
                              const QPointF& newLocalPos,
                              QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QGraphicsItem *m_item;
    QPointF m_oldPos;
    QPointF m_newPos;
};

} //Commands

#endif // ITEMPOSITIONCHANGECOMMAND_HPP
