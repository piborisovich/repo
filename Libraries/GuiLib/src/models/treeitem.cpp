#include "treeitem.hpp"

namespace GUI {

TreeItem::TreeItem(QVariantList data, TreeItem *parentItem) :
    m_parentItem(parentItem)
{
    int i = 0;
    for (auto it = data.begin(); it != data.end(); ++it) {
        m_itemData.insert(i++, qMakePair(*it, false));
    }
}

TreeItem::TreeItem(TreeItem *parentItem) :
    m_parentItem(parentItem)
{
}

void TreeItem::appendChild(TreeItem *child)
{
    m_childItems.push_back(std::shared_ptr<TreeItem>(child));
    child->m_parentItem = this;
}

void TreeItem::removeChild(int index)
{
    m_childItems.remove(index);
}

void TreeItem::removeChild(int index, int count)
{
    m_childItems.remove(index, count);
}

void TreeItem::insertChild(int index, TreeItem *child)
{
    m_childItems.insert( index, std::shared_ptr<TreeItem>(child) );
    child->m_parentItem = this;
}

void TreeItem::insertChild(int index, int count)
{
    m_childItems.insert( index, count, std::make_shared<TreeItem>(this) );
}

void TreeItem::moveChild(int from, int to)
{
    m_childItems.move(from, to);
}

TreeItem *TreeItem::child(int row)
{
    return row >= 0 && row < childCount() ? m_childItems.at(row).get() : nullptr;
}

int TreeItem::childCount() const
{
    return static_cast<int>(m_childItems.size());
}

int TreeItem::columnCount() const
{
    return int(m_itemData.size());
}

void TreeItem::setData(int column, const QVariant &data)
{
    if ( m_itemData.count(column) ) {
        m_itemData[column].first = data;
    } else {
        m_itemData[column] = qMakePair(data, false);
    }

}

void TreeItem::setData(int column, const QVariant &data, bool editable)
{
    m_itemData[column] = qMakePair(data, editable);
}

QVariant TreeItem::data(int column) const
{
    return m_itemData.value(column).first;
}

void TreeItem::setEditable(int column, bool editable)
{
    if ( m_itemData.count(column) ) {
        m_itemData[column].second = editable;
    }
}

bool TreeItem::editable(int column) const
{
    return m_itemData.count(column) ? m_itemData[column].second : false;
}

int TreeItem::row() const
{
    if (m_parentItem == nullptr) {
        return 0;
    }
    const auto it = std::find_if(m_parentItem->m_childItems.cbegin(), m_parentItem->m_childItems.cend(),
                                 [this](const std::shared_ptr<TreeItem> &treeItem) {
        return treeItem.get() == this;
    });

    if (it != m_parentItem->m_childItems.cend())
        return std::distance(m_parentItem->m_childItems.cbegin(), it);

    return -1;
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

void TreeItem::clear()
{
    m_childItems.clear();
}

} //GUI
