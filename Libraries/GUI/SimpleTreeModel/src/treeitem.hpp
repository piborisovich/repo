/**
 * @file treeitem.hpp
 * @brief Итем (строка) древовидного списка
 */

#ifndef TREEITEM_HPP
#define TREEITEM_HPP

#include <QSharedPointer>
#include <QVariant>

#if defined Q_OS_WIN32  || defined Q_OS_WIN64
#if defined MAKE_LIB
#define LIB_EXPORT Q_DECL_EXPORT
#else
#define LIB_EXPORT Q_DECL_IMPORT
#endif //MAKE_LIB
#endif //Q_OS_WIN32 || Q_OS_WIN64

namespace GUI {

#if defined Q_OS_WIN32
class LIB_EXPORT TreeItem
#else
class TreeItem
#endif
{
public:
    explicit TreeItem(QVariantList data, TreeItem *parentItem = nullptr);

    void appendChild(QSharedPointer<TreeItem> &&child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

private:
    QVector<QSharedPointer<TreeItem> > m_childItems;
    QVariantList m_itemData;
    TreeItem *m_parentItem;
};

} //GUI

#endif // TREEITEM_HPP
