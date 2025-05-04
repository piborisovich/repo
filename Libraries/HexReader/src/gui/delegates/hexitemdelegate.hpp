#ifndef HEXITEMDELEGATE_HPP
#define HEXITEMDELEGATE_HPP

#include <QStyledItemDelegate>

namespace HEX {

namespace GUI {

class HexItemDelegate : public QStyledItemDelegate
{
public:
    HexItemDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

} // GUI
} // HEX
#endif // HEXITEMDELEGATE_HPP
