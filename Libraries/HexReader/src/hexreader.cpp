#include "hexreader.hpp"

#include <QFile>
#include <QException>
#include <QDebug>

namespace HEX
{

const int HexReader::USBAHIndex = 0;
const uint32_t HexReader::USBAHShift = 12;
const int HexReader::USBALIndex = 0;
const uint32_t HexReader::USBALShift = 4;
const int HexReader::ULBAHIndex = 0;
const uint32_t HexReader::ULBAHShift = 24L;
const int HexReader::ULBALIndex = 1;
const uint32_t HexReader::ULBALShift = 16L;

HexReader::HexReader() : Interfaces::IHexReader(),
    m_baseAddress(0)
{
}

std::shared_ptr<HexReadonlyDataset> HexReader::readFile(const QString &fileName, int alignment)
{
    m_baseAddress = 0;
    m_state = Mode8;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << QObject::tr("File %1 can't be opened!").arg(fileName);

        throw QException();
    }

    auto result = std::make_shared<HexReadonlyDataset>();

    //=======Заполнение 0xFF пропусков по адресам до велечины выравнивания=======
    auto performAlign { [alignment, &result]( uint32_t effectiveAddress ) {

            if ( ( alignment == Alignment_16 || alignment == Alignment_32 )  && !result->data.empty() ) {

                if ( effectiveAddress - result->data.lastKey() < uint32_t(alignment) )  {
                    while ( effectiveAddress - result->data.lastKey() > 1 ) {
                        result->data.insert(result->data.lastKey() + 1, 0xFF);
                    }
                } else if ( effectiveAddress % uint32_t(alignment) ) {
                    auto addr = effectiveAddress;
                    while ( (addr--) % 4 ) {
                        result->data.insert(addr, 0xFF);
                    }
                }
            }
        }};
    //==================================================

    QTextStream fileStream(&file);

    uint32_t lineNumber = 1;
    QString readLine;
    while ( !fileStream.atEnd() ) {
        readLine = fileStream.readLine();

        try {
            Interfaces::IHexRecord* record = new HexRecord(readLine);

            if ( m_state == End ) {
                qCritical() << QObject::tr("Record after End of File record.");
                delete record;
                throw QException();
            }

            auto recordType = record->getRecordType();

            //EndOfFile
            if ( recordType == HexRecordType::EndOfFile ) {
                m_state = End;

                switch (m_state) {
                case Mode8:
                    result->mode = HexAddressingMode::Mode8bit;
                    break;
                case Mode16:
                    result->mode = HexAddressingMode::Mode16bit;
                    break;
                case Mode32:
                    result->mode = HexAddressingMode::Mode32bit;
                    break;
                default:;
                }

                delete record;
                continue;
            }

            // Switching modes
            switch (m_state) {
            case Mode8:
                if (( recordType == HexRecordType::ExtendedSegmentAddress ) ||
                    ( recordType == HexRecordType::StartSegmentAddress )) {
                    // 8 bit -> 16 bit
                    m_state = Mode16;
                } else if (( recordType == HexRecordType::ExtendedLinearAddress ) ||
                           ( recordType == HexRecordType::StartLinearAddress )) {
                    // 8bit -> 32 bit
                    m_state = HexReaderState::Mode32;
                }
                break;

            case Mode16:
                if (( recordType == HexRecordType::ExtendedLinearAddress ) ||
                    ( recordType == HexRecordType::StartLinearAddress)) {
                    // 16 bit -> 32 bit. NOT ALLOWED!
                    qCritical() << QObject::tr("Extended Linear Address or Start Linear Address in 16 bit mode.");
                    delete record;
                    throw QException();
                }
                break;
            case Mode32:
                if (( recordType == HexRecordType::ExtendedSegmentAddress ) ||
                    ( recordType == HexRecordType::StartSegmentAddress )) {
                    // 32 bit -> 16 bit. NOT ALLOWED!
                    qCritical() << QObject::tr("Extended Segment Address or Start Segment Address in 32 bit mode.");
                    delete record;
                    throw QException();
                }
                break;

            default:;
            }

            auto recordData = record->getData();
            auto recordOffset = record->getOffset();

            switch (recordType) {
            case HexRecordType::Data:
            {

                uint32_t effectiveAddress = 0;

                for (int index = 0; index < recordData.size(); index++) {

                    switch (m_state) {
                    case Mode8:
                        effectiveAddress = static_cast<uint32_t>(index + recordOffset);
                        break;

                    case Mode16:
                        effectiveAddress = m_baseAddress + static_cast<uint32_t>(index + recordOffset);
                        break;

                    case Mode32:
                        effectiveAddress = m_baseAddress + static_cast<uint32_t>(index + recordOffset);
                        break;

                    default:;
                    }

                    performAlign(effectiveAddress);

                    result->data.insert(effectiveAddress, recordData[index]);
                }
                break;
            }

            case HexRecordType::ExtendedSegmentAddress:
                // (16 bit mode)
                m_baseAddress = (static_cast<uint32_t>(recordData[USBAHIndex]) << USBAHShift) | (static_cast<uint32_t>(recordData[USBALIndex]) << USBALShift); // Lower bits 0-3 is 0x00
                break;

            case HexRecordType::ExtendedLinearAddress:
                // (32 bit mode)
                m_baseAddress = (static_cast<uint32_t>(recordData[ULBAHIndex]) << ULBAHShift) | (static_cast<uint32_t>(recordData[ULBALIndex]) << ULBALShift); // data contains two upper bytes, two lower is 0x00
                break;

            default:;
            }

            delete record;
        }
        catch (...)
        {
            qCritical() << QObject::tr("Error while processing line %1 : %2").arg(lineNumber).arg(readLine);

            file.close();

            result.reset();
            throw;
        }

        lineNumber++;
    }

    file.close();

    return result;
}


} //HEX
