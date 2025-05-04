#include "hexrecord.hpp"
#include "checksumcheckergenerator.hpp"
#include "hextostringcodec.hpp"

#include <QDebug>
#include <QException>

namespace HEX {

const QString HexRecord::lineRegexp = "^:[0-9ABCDEF]+";
const uint32_t HexRecord::reclenOffset = 0U;
const uint32_t HexRecord::reclenShift = 5U;
const uint32_t HexRecord::offsetOffset = 1U;
const uint32_t HexRecord::rectypeOffset = 3U;
const uint32_t HexRecord::zeroOffset = 0U;
const uint32_t HexRecord::endOfFileRecordReclen = 0U;
const uint32_t HexRecord::extendedSegmentAddressRecordReclen = 7U;
const uint32_t HexRecord::startSegmentAddressRecordReclen = 9U;
const uint32_t HexRecord::extendedLinearAddressRecordReclen = 7U;
const uint32_t HexRecord::startLinearAddressRecordReclen = 9U;
const uint32_t HexRecord::dataOffset = 4U;


HexRecord::HexRecord(const QString &data) : Interfaces::IHexRecord(data)
{
    m_lineRegExp.reset( new QRegularExpression(lineRegexp) );
    m_codec.reset(new HexToStringCodec());
    m_checksumCheckerGenerator.reset(new ChecksumCheckerGenerator());

    //Проверка на нечетное количество символов
    if ( (data.length() % 2) != 1 ) {
        signalizeLineIsNotParseable(data);
    }

    auto dataUpper = data.toUpper();
    auto match = m_lineRegExp->match(dataUpper);
    if ( !match.hasMatch() ) {
        signalizeLineIsNotParseable(data);
    }

    // Удаляем ":"
    dataUpper = dataUpper.right(dataUpper.length() - 1);

    // Чтение символов по парам
    QVector<uint8_t> lineData;

    for (int i = 0; i < dataUpper.length(); i += 2) {
        QString pair = dataUpper.mid(i, 2);
        lineData.append(m_codec->decode(pair));
    }

    // Проверка контрольной суммы
    if ( !m_checksumCheckerGenerator->check(lineData) ) {
        qCritical() << QObject::tr("Checksum error.");
        throw QException();
    }

    // Reading RECLEN (see format description)
    uint32_t reclen = lineData[reclenOffset];

    if ( reclen + reclenShift != static_cast<uint32_t>(lineData.size()) ) {
        qCritical() << QObject::tr("Incorrect record length. Got %1, expected %2").arg(reclen).arg(static_cast<uint32_t>(lineData.size()) - reclenShift);
        throw QException();
    }

    //Чтение смещения адреса
    m_offset = lineData[offsetOffset] * 0x100 + lineData[offsetOffset + 1];

    //Чтение типа записи
    uint8_t rectype = lineData[rectypeOffset];

    switch (rectype) {
    case static_cast<uint8_t>(HexRecordType::Data):
        m_recordType = HexRecordType::Data;
        break;

    case static_cast<uint8_t>(HexRecordType::EndOfFile):
        m_recordType = HexRecordType::EndOfFile;

        if (m_offset != zeroOffset) {
            qCritical() << QObject::tr("Incorrect OFFSET for End of File record. Expected %1, got %2").arg(zeroOffset).arg(m_offset);
            throw QException();
        }

        if (reclen != endOfFileRecordReclen) {
            qCritical() << QObject::tr("Incorrect RECLEN for End of File record. Expected %1, got %2").arg(endOfFileRecordReclen).arg(reclen);
            throw QException();
        }
        break;

    case static_cast<uint8_t>(HexRecordType::ExtendedSegmentAddress):
        m_recordType = HexRecordType::ExtendedSegmentAddress;

        if (m_offset != zeroOffset) {
            qCritical() << QObject::tr("Incorrect OFFSET for Extended Segment Address record. Expected %1, got %2").arg(zeroOffset).arg(m_offset);
            throw QException();
        }

        if (reclen != extendedSegmentAddressRecordReclen) {
            qCritical() << QObject::tr("Incorrect RECLEN for Extended Segment Address record. Expected %1, got %2").arg(extendedSegmentAddressRecordReclen).arg(reclen);
            throw QException();
        }
        break;

    case static_cast<uint8_t>(HexRecordType::StartSegmentAddress):
        m_recordType = HexRecordType::StartSegmentAddress;

        if (m_offset != zeroOffset) {
            qCritical() << QObject::tr("Incorrect OFFSET for Start Segment Address record. Expected %1, got %2").arg(zeroOffset).arg(m_offset);

            throw QException();
        }

        if (reclen != startSegmentAddressRecordReclen) {
            qCritical() << QObject::tr("Incorrect RECLEN for Start Segment Address record. Expected %1, got %2").arg(startSegmentAddressRecordReclen).arg(reclen);
            throw QException();
        }
        break;

    case static_cast<uint8_t>(HexRecordType::ExtendedLinearAddress):
        m_recordType = HexRecordType::ExtendedLinearAddress;

        if (m_offset != zeroOffset) {
            qCritical() << QObject::tr("Incorrect OFFSET for Extended Linear Address record. Expected %1, got %2").arg(zeroOffset).arg(m_offset);
            throw QException();
        }

        if (reclen + reclenShift != extendedLinearAddressRecordReclen) {
            qCritical() << QObject::tr("Incorrect RECLEN for Extended Linear Address record record. Expected %1, got %2").arg(extendedLinearAddressRecordReclen).arg(reclen);
            throw QException();
        }
        break;

    case static_cast<uint8_t>(HexRecordType::StartLinearAddress):
        m_recordType = HexRecordType::StartLinearAddress;

        if (m_offset != zeroOffset) {
            qCritical() << QObject::tr("Incorrect OFFSET for Start Linear Address record. Expected %1, got %2").arg(zeroOffset).arg(m_offset);
            throw QException();
        }

        if (reclen + reclenShift != startLinearAddressRecordReclen) {
            qCritical() << QObject::tr("Incorrect RECLEN for Start Linear Address record record. Expected %1, got %2").arg(startLinearAddressRecordReclen).arg(reclen);
            throw QException();
        }
        break;

    default:
        qCritical() << QObject::tr("Incorrect record type code: %1").arg(rectype);
        throw QException();
    }

    // Получить данные
    m_data = lineData.mid(dataOffset, static_cast<int>(int(static_cast<uint32_t>(lineData.length()) - reclenShift)));
}

HexRecordType HexRecord::getRecordType() const
{
    return m_recordType;
}

uint16_t HexRecord::getOffset() const
{
    return m_offset;
}

const QVector<uint8_t> &HexRecord::getData() const
{
    return m_data;
}

void HexRecord::signalizeLineIsNotParseable(const QString &data)
{
    qCritical() << QObject::tr("Line isn't parseable: \"%0\"").arg(data);

    throw QException();
}

} //HEXLib
