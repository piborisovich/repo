#ifndef HEXRECORD_HPP
#define HEXRECORD_HPP


#include "interfaces/IHexRecord.hpp"
#include "interfaces/IHexToStringCodec.hpp"
#include "interfaces/IChecksumCheckerGenerator.hpp"

#include <QRegularExpression>
#include <QScopedPointer>

namespace Hex
{
/*!
 * \brief Класс записи Hex
 */
class HexRecord : public Interfaces::IHexRecord
{
public:
    HexRecord(const QString &data);

    Interfaces::HexRecordType getRecordType() override;
    quint16 getOffset() override;
    QVector<quint8> getData() override;

    ~HexRecord() {}

private:

    static const QString LineRegexp;                         //!< Input string have to match to this regexp
    static const quint32 ReclenOffset;                       //!< RECLEN (Record Length) have this position in record data
    static const int ReclenShift;                            //!< Add this number to RECLEN, result have to be equal to record size
    static const quint32 OffsetOffset;                       //!< Most significant bit of OFFSET field position in record data
    static const quint32 RectypeOffset;                      //!< RECTYPE (Record Type) field position in record data
    static const quint32 ZeroOffset;                         //!< Offset for records with no data
    static const quint32 EndOfFileRecordReclen;              //!< Reclen value for End of File record
    static const quint32 ExtendedSegmentAddressRecordReclen; //!< Reclen value for Extended Segment Address record
    static const quint32 StartSegmentAddressRecordReclen;    //!< Reclen value for Start Segment Address record
    static const quint32 ExtendedLinearAddressRecordReclen;  //!< Reclen value for Extended Linear Address record
    static const quint32 StartLinearAddressRecordReclen;     //!< Reclen value for Start Linear Address record
    static const quint32 PayloadOffset;                      //!< Payload starts at this position in record data

    QScopedPointer<QRegularExpression> m_lineRegExp;                                  //!< Intel HEX string regexp matcher
    QScopedPointer<Interfaces::IHexToStringCodec> m_codec;                            //!< String<->Hex codec
    QScopedPointer<Interfaces::IChecksumCheckerGenerator> m_checksumCheckerGenerator; //!< Checksum checker

    quint16 m_offset;                       //!< Data offset (i.e. address) in record
    Interfaces::HexRecordType m_recordType; //!< Record type
    QVector<quint8> m_payload;              //!< Record payload

    /*!
     * \brief Проверка строки Hex на корректность
     * \param data
     */
    void SignalizeLineIsNotParseable(const QString &data);
};
} //Hex

#endif //HEXRECORD_HPP
