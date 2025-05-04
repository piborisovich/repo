#ifndef HEXVIEW_HPP
#define HEXVIEW_HPP

#include "../models/hexmodel.hpp"

#include <QTableView>

namespace HEX
{

namespace GUI {
/*!
 * \brief Представление данных Hex
 */
#ifdef LIB_EXPORT
class LIB_EXPORT HexView : public QTableView
#else
class HexView : public QTableView
#endif
{
    Q_OBJECT
public:
    HexView(QWidget *parent = nullptr);

    using Representations = HexModel::Representations;
signals:
    void loadError();

public:
    void load(const QString &path);

    QString path() const;

    /*!
     * \brief Получить данные HEX
     * \return адрес:данные
     */
    std::shared_ptr<HEX::HexReadonlyDataset> hexData() const;

    Representations representation() const;
    void setRepresentation(Representations representation);
};

} //GUI
} //HEX

#endif // HEXVIEW_HPP
