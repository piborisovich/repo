#include "hexmodel.hpp"

#include "../hexreader.hpp"

#include <QtMath>
#include <QException>
#include <QSize>

namespace HEX
{

HexModel::HexModel(QObject *parent) : QAbstractTableModel (parent),
    m_representation(Representations::BYTES_32),
    m_rowCount(0)
{
}

HexModel::~HexModel()
{
}

void HexModel::loadData(const QString &path)
{
    beginResetModel();
    clearModel();
    if ( !path.isEmpty() ) {
        try {
            HexReader reader;
            m_data = reader.readFile(path);
            rowCountCalc();
        } catch (QException &) {
            m_rowCount = 0;
            emit loadError();
        }
    }
    endResetModel();
    m_path = path;
}

int HexModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_rowCount;
}

int HexModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data ? int(m_representation) : 0;
}

QVariant HexModel::data(const QModelIndex &index, int role) const
{
    if ( m_data && !m_data->data.isEmpty() ) {
        if ( index.isValid() ) {
            uint32_t address = m_data->data.begin().key() + offset(index.row(), index.column());
            if ( role ==  Qt::DisplayRole ) {
                if ( m_data->data.contains( address ) ) {
                    return QString::number(m_data->data.value(address), 16).toUpper().rightJustified(2, '0');
                } else {
                    return QString("-");
                }
            }
            if ( role == Qt::TextAlignmentRole ) {
                return Qt::AlignCenter;
            }

            if ( role == Qt::ToolTipRole ) {
                return QString::number(address, 16).toUpper().rightJustified(8, '0');
            }
        }
    }
    return QVariant();
}

QVariant HexModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( m_data && !m_data->data.isEmpty() ) {
        if ( role == Qt::DisplayRole ) {
            if ( orientation == Qt::Vertical ) {

                if ( section >= 0 && section < m_rowCount ) {
                    return QString::number(m_data->data.begin().key() + offset(section, 0), 16).toUpper().rightJustified(8, '0');
                }
            } else {
                return QString::number(section, 16).rightJustified(2, '0').toUpper();
            }
        }
    }
    return QVariant();
}

void HexModel::clearModel()
{
    m_data.reset();
}

void HexModel::rowCountCalc()
{
    if ( m_data && m_data->data.count() ) {
        uint32_t addressBegin = m_data->data.firstKey();
        uint32_t addressEnd = m_data->data.lastKey();

        m_rowCount = qCeil(static_cast<qreal>(addressEnd - addressBegin + 1) / static_cast<qreal>(m_representation) );
    }
}

QString HexModel::path() const
{
    return m_path;
}

HexModel::Representations HexModel::representation() const
{
    return m_representation;
}

void HexModel::setRepresentation(Representations representation)
{
    if ( m_representation != representation ) {
        beginResetModel();
        m_representation = representation;
        rowCountCalc();
        endResetModel();
    }
}

int HexModel::getDataVolume() const
{
    return m_data ? m_data->data.size() : 0;
}

std::shared_ptr<HEX::HexReadonlyDataset> HexModel::hexData()
{
    return m_data;
}

uint32_t HexModel::offset(int row, int column) const
{
    return uint32_t(row * int(m_representation) + column);
}

} //HEX
