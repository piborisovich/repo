#ifndef TREEITEM_HPP
#define TREEITEM_HPP

#include "../gui_global.hpp"

#include <QDataStream>
#include <QException>
#include <QVariantList>
#include <QVector>

#include <QDebug>

#include <memory>

namespace GUI {

/*!
 * \brief Итем древовидного списка
 */

#ifdef LIB_EXPORT
class LIB_EXPORT TreeItem
#else
class TreeItem
#endif
{
public:
    TreeItem(QVariantList data, TreeItem *parentItem = nullptr);
    explicit TreeItem(TreeItem *parentItem = nullptr);

    void appendChild(TreeItem *child);
    void removeChild(int index);
    void removeChild(int index, int count);

    void insertChild(int index, TreeItem *child);
    void insertChild(int index, int count);

    void moveChild(int from, int to);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;

    void setData(int column, const QVariant &data);
    void setData(int column, const QVariant &data, bool editable);
    QVariant data(int column) const;

    void setEditable(int column, bool editable);
    bool editable(int column) const;

    int row() const;
    TreeItem *parentItem();

    void clear();

    friend QDataStream& operator<<(QDataStream &out, const TreeItem &item);
    friend QDataStream& operator>>(QDataStream &in, TreeItem &item);

private:
    QVector< std::shared_ptr<TreeItem> > m_childItems;
    QHash<int, QPair<QVariant, bool> > m_itemData; //!< Данные и редактируемость
    TreeItem *m_parentItem;
};



inline QDataStream& operator<<(QDataStream &out, const TreeItem &item)
{
    out << item.m_itemData.size();

    for ( auto it = item.m_itemData.cbegin(); it != item.m_itemData.cend(); it++ ) {
        out << it.key()
            << it.value();
    }

    out << item.m_childItems.size();

    for ( auto it = item.m_childItems.cbegin(); it != item.m_childItems.cend(); it++ ) {
        out << **it;
    }

    return out;
}

inline QDataStream& operator>>(QDataStream &in, TreeItem &item)
{
    int dataSize = 0;

    auto checkTransaction {
        [&in](){
            if ( !in.commitTransaction() ) {
                throw QException();
            }
        }
    };

    in.startTransaction();

    in >> dataSize;

    checkTransaction();

    for ( int i = 0; i < dataSize; ++i ) {
        int key;
        QPair<QVariant, bool> value;

        in.startTransaction();

        in >> key
           >> value;

        checkTransaction();

        item.m_itemData.insert(key, value);
    }

    int childCount = 0;

    in.startTransaction();

    in >> childCount;

    checkTransaction();

    for ( int i = 0; i < childCount; ++i ) {

        TreeItem *child = new TreeItem();

        in >> *child;

        item.appendChild(child);
    }

    return in;
}


} //GUI

#endif // TREEITEM_HPP
