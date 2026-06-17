#ifndef MACRODELETECOMMAND_HPP
#define MACRODELETECOMMAND_HPP

#include <QUndoCommand>
#include <QGraphicsScene>

namespace Commands
{

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

#endif // MACRODELETECOMMAND_HPP
