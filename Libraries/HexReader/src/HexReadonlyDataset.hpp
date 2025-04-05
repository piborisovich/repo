#ifndef HEXREADONLYDATASET_HPP
#define HEXREADONLYDATASET_HPP

#include <QMap>

#include "hex_global.hpp"
#include "hextypes.hpp"

namespace Hex
{

/*!
 * \brief Данные, извлеченные из Hex файла
 */
class LIB_EXPORT HexReadonlyDataset
{
public:
    HexAddressingMode Mode;
    QMap<quint32, quint8> Data;
};

} //Hex

#endif // HEXREADONLYDATASET_HPP
