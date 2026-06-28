#ifndef MACROADDCOMMAND_HPP
#define MACROADDCOMMAND_HPP

#include <QUndoCommand>
#include <QGraphicsScene>

namespace Commands
{

/*!
 * \brief Команда для ластика (может удалять сразу группу штрихов)
 */
class MacroAddCommand : public QUndoCommand {

public:
    MacroAddCommand(QGraphicsScene *scene, const QList<QGraphicsItem*> &items, QUndoCommand *parent = nullptr);

    ~MacroAddCommand() override;

    void undo() override;
    void redo() override;

private:
    QGraphicsScene *m_scene;
    QList<QGraphicsItem*> m_items;
    QList<bool> m_ownedStates; //!< Наличие на сцене
};

} //Commands

#endif // MACROADDCOMMAND_HPP
