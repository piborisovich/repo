#include "smartitemlistdelegate.hpp"

#include <QPainter>

namespace GUI {



SmartItemListDelegate::SmartItemListDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    m_uniformHeight(0)
{
}

SmartItemListDelegate::~SmartItemListDelegate()
{
}

void SmartItemListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    if ( option.state & QStyle::State_Selected ) {
        painter->fillRect(option.rect, option.palette.highlight());
    } else {
        painter->fillRect(option.rect, option.palette.base());
    }

    painter->setPen(option.palette.text().color());

    painter->drawText(option.rect.adjusted(6, 4, -6, -4),
                      Qt::AlignCenter | Qt::TextWordWrap,
                      index.data().toString());

    painter->restore();
}

QSize SmartItemListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ensureCalculated(option, index.model());

    QFontMetrics fm(option.font);
    QString text = index.data().toString();

    QRect rect = fm.boundingRect(
        QRect(0, 0, maxWidth, 1000),
        Qt::TextWordWrap,
        text );

    int w = qBound(minWidth, rect.width() + hPadding, maxWidth);

    return QSize(w, m_uniformHeight);
}

void SmartItemListDelegate::ensureCalculated(const QStyleOptionViewItem &option, const QAbstractItemModel *model) const
{
    if ( m_uniformHeight > 0 ) {
        return;
    }

    QFontMetrics fm(option.font);
    int maxH = 0;

    for ( int r = 0; r < model->rowCount(); ++r ) {
        QString text = model->index(r, 0).data().toString();
        QRect rect = fm.boundingRect(
            QRect(0, 0, maxWidth, 1000),
            Qt::TextWordWrap,
            text );
        maxH = qMax(maxH, rect.height());
    }

    m_uniformHeight = maxH + vPadding;
}

} //GUI
