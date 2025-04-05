#ifndef HEXMODEL_HPP
#define HEXMODEL_HPP

#include <QAbstractTableModel>

namespace Hex {

class HexModel : public QAbstractTableModel
{
public:
    explicit HexModel(QObject *parent = nullptr);
};

} //Hex

#endif // HEXMODEL_HPP
