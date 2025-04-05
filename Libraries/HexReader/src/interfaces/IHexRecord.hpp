#ifndef IHEXRECORD_HPP
#define IHEXRECORD_HPP

#include <QVector>
#include <QString>

namespace Hex
{
namespace Interfaces
{

/*!
 * \brief Возможные типы записей Intel Hex
 */
enum HexRecordType
{
    Data					= 0x00,
    EndOfFile				= 0x01,
    ExtendedSegmentAddress	= 0x02,
    StartSegmentAddress		= 0x03,
    ExtendedLinearAddress	= 0x04,
    StartLinearAddress		= 0x05
};


/*!
 * \brief Интерфейс для записи Hex (строка в файле)
 */
class IHexRecord
{
public:

    IHexRecord(const QString &data)
    {
        Q_UNUSED(data);
    }

    virtual HexRecordType getRecordType() = 0;

    virtual quint16 getOffset() = 0;

    virtual QVector<quint8> getData() = 0;

    virtual ~IHexRecord() {}
};
}
} //Hex


#endif // IHEXRECORD_HPP
