#include "macroaddcommand.hpp"

#include <QGraphicsItem>

namespace Commands
{

MacroAddCommand::MacroAddCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &items, QUndoCommand *parent) : QUndoCommand(parent),
    m_scene(scene),
    m_items(items)
{
    setText("Add figure");

    for ( int i = 0; i < m_items.size(); ++i ) {
        m_ownedStates.append(true);
    }
}

MacroAddCommand::~MacroAddCommand()
{
    for ( int i = 0; i < m_items.size(); ++i ) {
        if (!m_ownedStates[i]) {
            delete m_items[i];
        }
    }
}

void MacroAddCommand::undo()
{
    for ( int i = 0; i < m_items.size(); ++i ) {
        m_scene->removeItem(m_items[i]);
        m_ownedStates[i] = false;
    }
}

void MacroAddCommand::redo()
{
    for ( int i = 0; i < m_items.size(); ++i ) {
        if ( !m_ownedStates[i] ) {
            m_scene->addItem(m_items[i]);
            m_ownedStates[i] = true;
        }
    }
}

} //Commands
