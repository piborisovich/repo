#include "commands.hpp"

namespace Commands
{

AddItemCommand::AddItemCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent) : QUndoCommand(parent),
    m_scene(scene),
    m_item(item),
    m_isOwnedByScene(true)
{
        setText("Add figure");
}

AddItemCommand::~AddItemCommand()
{
    // Если элемент сейчас не на сцене (в состоянии Undo), удаляем его из памяти совсем
    if (!m_isOwnedByScene) {
        delete m_item;
    }
}

void AddItemCommand::undo()
{
    m_scene->removeItem(m_item);
    m_isOwnedByScene = false;
}

void AddItemCommand::redo()
{
    m_scene->addItem(m_item);
    m_isOwnedByScene = true;
}

MacroDeleteCommand::MacroDeleteCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &items, QUndoCommand *parent) : QUndoCommand(parent),
    m_scene(scene),
    m_items(items)
{
    setText("Eraser");

    for ( int i = 0; i < m_items.size(); ++i ) {
        m_ownedStates.append(true);
    }
}

MacroDeleteCommand::~MacroDeleteCommand()
{
    for ( int i = 0; i < m_items.size(); ++i ) {
        if (!m_ownedStates[i]) {
            delete m_items[i];
        }
    }
}

void MacroDeleteCommand::undo()
{
    for ( int i = 0; i < m_items.size(); ++i ) {
        m_scene->addItem(m_items[i]);
        m_ownedStates[i] = true;
    }
}

void MacroDeleteCommand::redo()
{
    for ( int i = 0; i < m_items.size(); ++i ) {
        m_scene->removeItem(m_items[i]);
        m_ownedStates[i] = false;
    }
}
} //Commands
