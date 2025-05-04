#ifndef IHEXRECORD_HPP
#define IHEXRECORD_HPP

#include "../hex_global.hpp"

#include <QVector>
#include <QString>

namespace HEX
{

/*!
 * \brief Типы записей Intel Hex
 */
enum class HexRecordType
{
    Data			= 0x00,
    EndOfFile			= 0x01,
    ExtendedSegmentAddress	= 0x02,
    StartSegmentAddress		= 0x03,
    ExtendedLinearAddress	= 0x04,
    StartLinearAddress		= 0x05
};

namespace Interfaces
{
/*!
 * \brief Интерфейс записи Intel Hex (строка в файле)
 */
#ifdef LIB_EXPORT
class LIB_EXPORT IHexRecord
#else
class IHexRecord
#endif
{
public:
    /*!
     * \brief Создание записи из строки файла .hex
     * \param data
     */
    IHexRecord(const QString &data)
    {
        Q_UNUSED(data);
    }

    virtual HexRecordType getRecordType() const = 0;
    virtual uint16_t getOffset() const = 0;
    virtual const QVector<uint8_t> &getData() const = 0;

    virtual ~IHexRecord() {}
};
}
} //HEX


#endif //IHEXRECORD_HPP
