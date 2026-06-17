#include "additemcommand.hpp"

#include <QGraphicsItem>

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

} //Commands
