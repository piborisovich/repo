#pragma once

#include "gui_global.hpp"

#include <QStyledItemDelegate>

namespace GUI {

/*!
 * \brief Окно справки
 */
class WIDGETS_EXPORT SmartItemListDelegate : public QStyledItemDelegate

{
    Q_OBJECT
public:
    explicit SmartItemListDelegate(QObject *parent = nullptr);
    ~SmartItemListDelegate() override;

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    void ensureCalculated(const QStyleOptionViewItem &option, const QAbstractItemModel *model) const;

private:
    mutable int m_uniformHeight;

    const int minWidth = 80;
    const int maxWidth = 200;
    const int hPadding = 12;
    const int vPadding =10;

};

} //GUI
