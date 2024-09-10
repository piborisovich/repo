#include "treeitem.hpp"

namespace GUI {

TreeItem::TreeItem(QVariantList data, TreeItem *parentItem) :
    m_itemData(std::move(data)), m_parentItem(parentItem)
{
}

void TreeItem::appendChild(QSharedPointer<TreeItem> &&child)
{
    m_childItems.push_back(QSharedPointer<TreeItem>(child));
}

TreeItem *TreeItem::child(int row)
{
    return row >= 0 && row < childCount() ? m_childItems.at(row).get() : nullptr;
}

int TreeItem::childCount() const
{
    return int(m_childItems.size());
}

int TreeItem::columnCount() const
{
    return int(m_itemData.count());
}

QVariant TreeItem::data(int column) const
{
    return m_itemData.value(column);
}

int TreeItem::row() const
{
    if (m_parentItem == nullptr)
        return 0;
    const auto it = std::find_if(m_parentItem->m_childItems.cbegin(), m_parentItem->m_childItems.cend(),
                                 [this](const QSharedPointer<TreeItem> &treeItem) {
                                     return treeItem.get() == this;
                                 });

    if (it != m_parentItem->m_childItems.cend())
        return std::distance(m_parentItem->m_childItems.cbegin(), it);
    Q_ASSERT(false); // should not happen
    return -1;
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

} //GUI
