#include "treemodel.hpp"

#include <Core/Utils>

#include <QStringView>

namespace GUI {

using Core::Utils::secure_connect;

TreeModel::TreeModel(QVariantList headers, QObject *parent) :
    QAbstractItemModel(parent),
    rootItem(new TreeItem(headers))
{
    secure_connect(this,
                   &TreeModel::dataChanged,
                   this,
                   &TreeModel::on_dataChanged);

    secure_connect(this,
                   &TreeModel::rowsRemoved,
                   this,
                   &TreeModel::on_rowsRemoved);

    secure_connect(this,
                   &TreeModel::rowsInserted,
                   this,
                   &TreeModel::on_rowsInserted);
}

TreeModel::~TreeModel()
{
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if ( index.isValid() ) {
        if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
            const auto *item = static_cast<const TreeItem*>(index.internalPointer());
            return item->data(index.column());
        }
    }

    return QVariant();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( index.isValid() && role == Qt::EditRole ) {
        auto *item = static_cast<TreeItem*>(index.internalPointer());
        if ( item ) {
            if ( item->data(index.column()) != value ) {
                item->setData(index.column(), value, true);
                emit dataChanged(index, index);
                return true;
            }
        }
    }

    return false;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if ( index.isValid() ) {
        if ( index.internalPointer() != nullptr ) {
            TreeItem *item = reinterpret_cast<TreeItem*>(index.internalPointer());
            if ( item->editable(index.column()) ) {
                return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
            }

        }
        return QAbstractItemModel::flags(index);
    }

    return Qt::ItemFlags(Qt::NoItemFlags);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return orientation == Qt::Horizontal && role == Qt::DisplayRole
            ? rootItem->data(section) : QVariant{};
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) ) {
        return {};
    }

    TreeItem *parentItem = parent.isValid()
            ? static_cast<TreeItem*>(parent.internalPointer())
            : rootItem.get();

    if (auto *childItem = parentItem->child(row))
        return createIndex(row, column, childItem);
    return {};
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    auto *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    return parentItem != rootItem.get()
            ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex{};
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    const TreeItem *parentItem = parent.isValid()
            ? static_cast<const TreeItem*>(parent.internalPointer())
            : rootItem.get();

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return rootItem->columnCount();
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    TreeItem *parentItem = parent.isValid()
            ? reinterpret_cast<TreeItem*>(parent.internalPointer())
            : rootItem.get();

    if ( parentItem && count > 0 && row >= 0 ) {
        row = qMin( row, rowCount(parent) - 1 );

        beginInsertRows(parent, row, row + count - 1);

        parentItem->insertChild(row, count);

        endInsertRows();
    }

    return false;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TreeItem *parentItem = parent.isValid()
            ? reinterpret_cast<TreeItem*>(parent.internalPointer())
            : rootItem.get();

    count = qMin( count, rowCount(parent) - row );

    if ( parentItem && count > 0 ) {

        beginRemoveRows(parent, row, row + count - 1);

        parentItem->removeChild(row, count);

        endRemoveRows();

        return true;

    }

    return false;
}

} //CANLoader
