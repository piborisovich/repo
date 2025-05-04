#include "hexview.hpp"
#include "hexitemdelegate.hpp"

#include <QHeaderView>

#include <QAbstractButton>

namespace HEX
{

namespace GUI {

HexView::HexView(QWidget *parent) : QTableView(parent)
{
    HexModel *hexModel = new HexModel(this);

    auto result = QObject::connect(hexModel,
                                   &HexModel::loadError,
                                   this,
                                   &HexView::loadError);
    Q_ASSERT(result);

    setModel(hexModel);
    setItemDelegate(new HexItemDelegate(this));

    setShowGrid(false);
    setSelectionMode(QAbstractItemView::SingleSelection);\
}

void HexView::load(const QString &path)
{
    HexModel *hexModel = static_cast<HexModel*>(model());
    hexModel->loadData(path);
    int dataVol = hexModel->getDataVolume();

    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto button = findChild<QAbstractButton*>(QString(), Qt::FindDirectChildrenOnly);

    if ( button ) {
        if ( dataVol ) {
            button->setToolTip(QString("%1 Байт").arg(dataVol));
        } else {
            button->setToolTip("");
        }
    }
}

QString HexView::path() const
{
    HexModel *hexModel = static_cast<HexModel*>(model());
    return hexModel->path();
}

std::shared_ptr<HEX::HexReadonlyDataset> HexView::hexData() const
{
    HexModel *hexModel = static_cast<HexModel*>(model());

    return hexModel->hexData();
}

HexView::Representations HexView::representation() const
{
    HexModel *hexModel = static_cast<HexModel*>(model());
    return  hexModel->representation();
}

void HexView::setRepresentation(HexView::Representations representation)
{
    HexModel *hexModel = static_cast<HexModel*>(model());
    hexModel->setRepresentation(representation);
}

} //GUI
} //HEX
