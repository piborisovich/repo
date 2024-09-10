/**
 * @file treemodel.hpp
 * @brief Модель древовидного списка
 */

#ifndef TREEMODEL_HPP
#define TREEMODEL_HPP

#include "treeitem.hpp"

#include <QAbstractItemModel>

namespace GUI {

#if defined Q_OS_WIN32
class LIB_EXPORT TreeModel : public QAbstractItemModel
#else
class TreeModel : public QAbstractItemModel
#endif
{
    Q_OBJECT
public:
    Q_DISABLE_COPY(TreeModel)

    explicit TreeModel(const QVariantList &headers, QObject *parent = nullptr);
    ~TreeModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

protected:
    QSharedPointer<TreeItem> rootItem;
};

} //GUI

#endif // TREEMODEL_HPP
