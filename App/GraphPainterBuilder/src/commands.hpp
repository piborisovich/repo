#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace Commands
{

/*!
 * \brief Команда для отмены операции добавления итема и возврата (UNDO/REDO)
 */
class AddItemCommand : public QUndoCommand {

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

/*!
 * \brief Команда для ластика (может удалять сразу группу штрихов)
 */
class MacroDeleteCommand : public QUndoCommand {

public:
    MacroDeleteCommand(QGraphicsScene *scene, const QList<QGraphicsItem*> &items, QUndoCommand *parent = nullptr);

    ~MacroDeleteCommand() override;

    void undo() override;
    void redo() override;

private:
    QGraphicsScene *m_scene;
    QList<QGraphicsItem*> m_items;
    QList<bool> m_ownedStates;
};

} //Commands


#endif // COMMANDS_HPP
