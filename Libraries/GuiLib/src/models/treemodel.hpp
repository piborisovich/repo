#ifndef CANINFOMODEL_HPP
#define CANINFOMODEL_HPP

#include "treeitem.hpp"

#include <QAbstractItemModel>

#include <memory>

namespace GUI {

/*!
 * \brief Модель древовидного списка
 */
#ifdef LIB_EXPORT
class LIB_EXPORT TreeModel : public QAbstractItemModel
#else
class TreeModel : public QAbstractItemModel
#endif
{
    Q_OBJECT
public:
    Q_DISABLE_COPY(TreeModel)

    enum Columns {
        Column1,
        Column2,
        Column3,
        Column4,
        Column5,
        Column6,
        Column7,
        Column8,
        Column9,
        Column10,
    };

    explicit TreeModel(QVariantList headers, QObject *parent = nullptr);
    ~TreeModel() override;

    virtual const QVariantList &headers() const = 0;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

    bool insertRows( int row, int count, const QModelIndex &parent = QModelIndex() ) override;
    bool removeRows( int row, int count, const QModelIndex &parent = QModelIndex() ) override;

protected slots:
   virtual void on_rowsInserted(const QModelIndex &parent, int first, int last) = 0;
   virtual void on_rowsRemoved(const QModelIndex &parent, int first, int last) = 0;
   virtual void on_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) = 0;

protected:
    std::unique_ptr<TreeItem> rootItem;
};

} //CANLoader
#endif // CANINFOMODEL_HPP
