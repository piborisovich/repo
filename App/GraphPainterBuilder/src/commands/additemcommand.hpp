#ifndef ADDITEMCOMMAND_HPP
#define ADDITEMCOMMAND_HPP

#include <QUndoCommand>
#include <QGraphicsScene>

namespace Commands
{

/*!
 * \brief Команда для отмены операции добавления итема и возврата (UNDO/REDO)
 */
class AddItemCommand : public QUndoCommand
{

public:
    AddItemCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent = nullptr);

    ~AddItemCommand();

    void undo() override;
    void redo() override;

private:
    QGraphicsScene *m_scene;
    QGraphicsItem *m_item;
    bool m_isOwnedByScene;
};

} //Commands


#endif // ADDITEMCOMMAND_HPP
