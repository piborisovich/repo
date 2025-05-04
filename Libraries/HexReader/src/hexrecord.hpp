#ifndef HEXRECORD_HPP
#define HEXRECORD_HPP

#include "interfaces/ihexrecord.hpp"
#include "interfaces/ihextostringcodec.hpp"
#include "interfaces/ichecksumcheckergenerator.hpp"

#include <QRegularExpression>
#include <QObject>

#include <memory>

namespace HEX
{
/*!
 * \brief Запись Intel Hex (строка в файле)
 */
#ifdef LIB_EXPORT
class LIB_EXPORT HexRecord : public Interfaces::IHexRecord
#else
class HexRecord : public Interfaces::IHexRecord
#endif
{
public:
    HexRecord(const QString &data);
    virtual ~HexRecord() = default;

    HexRecordType getRecordType() const;
    uint16_t getOffset() const;
    const QVector<uint8_t> &getData() const;

private:

    static const QString lineRegexp;                         //!< Регулярное выражение для парсинга строки
    static const uint32_t reclenOffset;                       //!< Смещение поля RECLEN в массиве байт, полученом из строки
    static const uint32_t reclenShift;                        //!< Разница между числом байт в строке и RECLEN
    static const uint32_t offsetOffset;                       //!< Смещение поля OFFSET в массиве байт, полученом из строки
    static const uint32_t rectypeOffset;                      //!< Смещение поля с типом записи в массиве байт, полученом из строки
    static const uint32_t zeroOffset;                         //!< Нулевое смещение адреса
    static const uint32_t endOfFileRecordReclen;              //!< RECLEN для записи типа EndOfFile
    static const uint32_t extendedSegmentAddressRecordReclen; //!< RECLEN для записи типа ExtendedSegmentAddress
    static const uint32_t startSegmentAddressRecordReclen;    //!< RECLEN для записи типа StartSegmentAddress
    static const uint32_t extendedLinearAddressRecordReclen;  //!< RECLEN для записи типа ExtendedLinearAddress
    static const uint32_t startLinearAddressRecordReclen;     //!< RECLEN для записи типа StartLinearAddress
    static const uint32_t dataOffset;                         //!<Смещение данных в массиве байт, полученом из строки

    std::unique_ptr<QRegularExpression> m_lineRegExp;
    std::unique_ptr<Interfaces::IHexToStringCodec> m_codec;
    std::unique_ptr<Interfaces::IChecksumCheckerGenerator> m_checksumCheckerGenerator;

    uint16_t m_offset;          //!< Адрес
    HexRecordType m_recordType; //!< Тип записи
    QVector<uint8_t> m_data;    //!< Данные

[[noreturn]] void signalizeLineIsNotParseable(const QString &data); //! Проверка строки на корректность
};

} //HEX

#endif // HEXRECORD_HPP
