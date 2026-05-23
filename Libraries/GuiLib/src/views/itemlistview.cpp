#include "itemlistview.hpp"
#include "../delegates/smartitemlistdelegate.hpp"

#include <QStandardItemModel>

namespace GUI {

static const int SPACING_PX = 5;

ItemListView::ItemListView(const QStringList &strings, QWidget *parent) :
    QListView(parent)
{
    QStandardItemModel *itemModel = new QStandardItemModel(this);
    setItemDelegate(new SmartItemListDelegate(this));
    setModel(itemModel);

    for ( const QString &str : strings ) {
        itemModel->appendRow(new QStandardItem(str));
    }

    setViewMode(QListView::IconMode);
    setFlow(QListView::LeftToRight);
    setWrapping(true);
    setResizeMode(QListView::Adjust);
    setSpacing(SPACING_PX);
    setItemAlignment(Qt::AlignCenter);

    setGridSize(QSize());
}

ItemListView::~ItemListView()
{
}

} //GUI
