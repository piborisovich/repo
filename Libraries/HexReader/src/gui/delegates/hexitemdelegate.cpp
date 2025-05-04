#include "hexitemdelegate.hpp"

#include <QPainter>

#include <QDebug>

namespace HEX {

namespace GUI {


HexItemDelegate::HexItemDelegate(QObject *parent) : QStyledItemDelegate (parent)
{
}

void HexItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.isValid() ) {

        QString dataStr = index.data().toString();

        painter->setFont(option.font);

        if ( (option.state & QStyle::State_Selected) ) {

            QRect rect = QRect(0, 0, option.rect.height() - 5, option.rect.height() - 5);
            rect.moveCenter(option.rect.center());

            QColor gray(Qt::gray);
            QColor white(Qt::white);

            painter->setRenderHint(QPainter::Antialiasing);
            painter->setBrush(gray);
            painter->setPen(white);

            painter->drawRoundedRect(rect, 5, 5, Qt::AbsoluteSize);
            painter->drawText(rect, int(Qt::AlignCenter), dataStr);
            return;
        }
    }

    QStyledItemDelegate::paint(painter, option, index);
}

QSize HexItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

} //GUI
} //HEX
