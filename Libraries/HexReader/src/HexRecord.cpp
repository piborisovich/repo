#include "HexRecord.hpp"

#include "HexToStringCodec.hpp"
#include "ChecksumCheckerGenerator.hpp"

#include <QException>

namespace Hex
{

const QString HexRecord::LineRegexp = "^:[0-9ABCDEF]+";
const quint32 HexRecord::ReclenOffset = 0U;
const int HexRecord::ReclenShift = 5;
const quint32 HexRecord::OffsetOffset = 1U;
const quint32 HexRecord::RectypeOffset = 3U;
const quint32 HexRecord::ZeroOffset = 0U;
const quint32 HexRecord::EndOfFileRecordReclen = 0U;
const quint32 HexRecord::ExtendedSegmentAddressRecordReclen = 7U;
const quint32 HexRecord::StartSegmentAddressRecordReclen = 9U;
const quint32 HexRecord::ExtendedLinearAddressRecordReclen = 7U;
const quint32 HexRecord::StartLinearAddressRecordReclen = 9U;
const quint32 HexRecord::PayloadOffset = 4U;

HexRecord::HexRecord(const QString &data) : Interfaces::IHexRecord(data)
{
    m_lineRegExp.reset( new QRegularExpression(LineRegexp) );
    m_codec.reset( new HexToStringCodec() );
    m_checksumCheckerGenerator.reset( new ChecksumCheckerGenerator() );

    // We need to have odd characters number
    if ((data.length() % 2) != 1) {
        SignalizeLineIsNotParseable(data);
    }

    auto dataUpper = data.toUpper();
    if ( !m_lineRegExp->match(dataUpper).hasMatch() ) {
        SignalizeLineIsNotParseable(data);
    }

    // Removing ":" from begin
    dataUpper = dataUpper.right(dataUpper.length() - 1);

    // Reading characters by pairs
    QVector<quint8> lineData;
    auto byteIterator = dataUpper.begin();

    while (byteIterator != dataUpper.end())
    {
        QString pair;
        pair.append(*byteIterator);
        byteIterator ++;
        pair.append(*byteIterator);
        byteIterator ++;

        lineData.append(m_codec->decode(pair));
    }

    // Checking checksum
    if ( !m_checksumCheckerGenerator->check(lineData) ) {
        qCritical() << QObject::tr("Checksum error.");
        throw QException();
    }

    // Reading RECLEN (see format description)
    uint reclen = lineData[ReclenOffset];

    if (reclen + ReclenShift != (uint)lineData.size())
    {
        qCritical() << QObject::tr("Incorrect record length. Got %1, expected %2").arg(reclen).arg(lineData.size() - ReclenShift);

        throw QException();
    }

    // Reading offset
    m_offset = lineData[OffsetOffset] * 0x100 + lineData[OffsetOffset + 1];

    // Detecting record type
    quint8 rectype = lineData[RectypeOffset];

    switch (rectype)
    {
    // Data
    case Interfaces::HexRecordType::Data:
        m_recordType = Interfaces::HexRecordType::Data;
        break;

        // End of file
    case Interfaces::HexRecordType::EndOfFile:
        m_recordType = Interfaces::HexRecordType::EndOfFile;

        if (m_offset != ZeroOffset) {
            qCritical() << QObject::tr("Incorrect OFFSET for End of File record. Expected %1, got %2").arg(ZeroOffset).arg(m_offset);

            throw QException();
        }

        if (reclen != EndOfFileRecordReclen) {
            qCritical() << QObject::tr("Incorrect RECLEN for End of File record. Expected %1, got %2").arg(EndOfFileRecordReclen).arg(reclen);

            throw QException();
        }
        break;

        // Extended segment address
    case Interfaces::HexRecordType::ExtendedSegmentAddress:
        m_recordType = Interfaces::HexRecordType::ExtendedSegmentAddress;

        if (m_offset != ZeroOffset)  {
            qCritical() << QObject::tr("Incorrect OFFSET for Extended Segment Address record. Expected %1, got %2").arg(ZeroOffset).arg(m_offset);

            throw QException();
        }

        if (reclen != ExtendedSegmentAddressRecordReclen) {
            qCritical() << QObject::tr("Incorrect RECLEN for Extended Segment Address record. Expected %1, got %2").arg(ExtendedSegmentAddressRecordReclen).arg(reclen);

            throw QException();
        }
        break;

        // Start segment address
    case Interfaces::HexRecordType::StartSegmentAddress:
        m_recordType = Interfaces::HexRecordType::StartSegmentAddress;

        if (m_offset != ZeroOffset) {
            qCritical() << QObject::tr("Incorrect OFFSET for Start Segment Address record. Expected %1, got %2").arg(ZeroOffset).arg(m_offset);

            throw QException();
        }

        if (reclen != StartSegmentAddressRecordReclen) {
            qCritical() << QObject::tr("Incorrect RECLEN for Start Segment Address record. Expected %1, got %2").arg(StartSegmentAddressRecordReclen).arg(reclen);

            throw QException();
        }
        break;

        // Extended linear address
    case Interfaces::HexRecordType::ExtendedLinearAddress:
        m_recordType = Interfaces::HexRecordType::ExtendedLinearAddress;

        if (m_offset != ZeroOffset) {
            qCritical() << QObject::tr("Incorrect OFFSET for Extended Linear Address record. Expected %1, got %2").arg(ZeroOffset).arg(m_offset);

            throw QException();
        }

        if (reclen != ExtendedLinearAddressRecordReclen)
        {
            qCritical() << QObject::tr("Incorrect RECLEN for Extended Linear Address record record. Expected %1, got %2").arg(ExtendedLinearAddressRecordReclen).arg(reclen);

            throw QException();
        }
        break;

        // Start linear address
    case Interfaces::HexRecordType::StartLinearAddress:
        m_recordType = Interfaces::HexRecordType::StartLinearAddress;

        if (m_offset != ZeroOffset)
        {
            qCritical() << QObject::tr("Incorrect OFFSET for Start Linear Address record. Expected %1, got %2").arg(ZeroOffset).arg(m_offset);

            throw QException();
        }

        if (reclen != StartLinearAddressRecordReclen)
        {
            qCritical() << QObject::tr("Incorrect RECLEN for Start Linear Address record record. Expected %1, got %2").arg(StartLinearAddressRecordReclen).arg(reclen);

            throw QException();
        }
        break;

    default:
        qCritical() << QObject::tr("Incorrect record type code: %1").arg(rectype);

        throw QException();
        break;
    }

    // Getting payload
    m_payload = lineData.mid(PayloadOffset, lineData.length() - ReclenShift);
}

Interfaces::HexRecordType HexRecord::getRecordType()
{
    return m_recordType;
}

quint16 HexRecord::getOffset()
{
    return m_offset;
}

QVector<quint8> HexRecord::getData()
{
    return m_payload;
}

void HexRecord::SignalizeLineIsNotParseable(const QString &data)
{
    qCritical() << QObject::tr("Line isn't parseable: \"%0\"").arg(data);

    throw QException();
}

} //Hex
