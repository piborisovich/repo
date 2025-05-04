#pragma once

#include "../hexreadonlydataset.hpp"

#include <QAbstractTableModel>

#include <memory>

namespace HEX
{

/*!
 * \brief Модель отображения HEX данных
 */
#ifdef LIB_EXPORT
class LIB_EXPORT HexModel : public QAbstractTableModel
#else
class HexModel : public QAbstractTableModel
#endif
{
    Q_OBJECT
public:
    HexModel(QObject *parent = nullptr);
    ~HexModel() override;

    enum class Representations
    {
        BYTES_8 = 8,
        BYTES_16 = 16,
        BYTES_32 = 32
    };

signals:
    void loadError();

public:

    void loadData(const QString &path);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    Representations representation() const;
    void setRepresentation(Representations representation);

    int getDataVolume() const;

    /*!
     * \brief Получить данные HEX
     * \return адрес:данные
     */
    std::shared_ptr<HEX::HexReadonlyDataset> hexData();

    QString path() const;

private:
    uint32_t offset(int row, int column) const;
    void clearModel();
    void rowCountCalc();

private:
    std::shared_ptr<HEX::HexReadonlyDataset> m_data;
    Representations m_representation;
    int m_rowCount;
    QString m_path;
};

} //HEX
