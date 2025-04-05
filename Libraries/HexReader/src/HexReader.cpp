#include "HexReader.hpp"

#include "interfaces/IHexRecord.hpp"
#include "HexRecord.hpp"
#include "hextypes.hpp"

#include <QDebug>
#include <QException>
#include <QFile>
#include <QTextStream>

namespace Hex
{

const int HexReader::USBAHIndex = 0;
const quint32 HexReader::USBAHShift = 12;
const int HexReader::USBALIndex = 0;
const quint32 HexReader::USBALShift = 4;
const int HexReader::ULBAHIndex = 0;
const quint32 HexReader::ULBAHShift = 24L;
const int HexReader::ULBALIndex = 1;
const quint32 HexReader::ULBALShift = 16L;

HexReadonlyDataset* HexReader::readFile(const QString &fileName)
{
    // Reset state
    m_baseAddress = 0;
    m_state = HexReaderState::Mode8;

    // Reading file line by line
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << QObject::tr("File %1 can't be opened!").arg(fileName);

        throw QException();
    }

    HexReadonlyDataset* result = new HexReadonlyDataset();

    QTextStream fileStream(&file);

    quint32 lineNumber = 0;
    QString readLine;
    while ( !fileStream.atEnd() )
    {
        readLine = fileStream.readLine();

        try {
            Interfaces::IHexRecord* record = new HexRecord(readLine);

            if (HexReaderState::End == m_state) {
                // New record after end
                qCritical() << QObject::tr("Record after End of File record.");
                delete record;
                throw new QException();
            }

            auto recordType = record->getRecordType();

            // Maybe end of file?
            if (Interfaces::HexRecordType::EndOfFile == recordType)
            {
                m_state = HexReaderState::End;

                // Saving mode into result
                switch (m_state)
                {
                case HexReaderState::Mode8:
                    result->Mode = HexAddressingMode::Mode8bit;
                    break;
                case HexReaderState::Mode16:
                    result->Mode = HexAddressingMode::Mode16bit;
                    break;
                case HexReaderState::Mode32:
                    result->Mode = HexAddressingMode::Mode32bit;
                    break;
                default:;
                }

                delete record;
                continue;
            }

            // Switching modes
            switch (m_state) {

            case HexReaderState::Mode8:
                if ((Interfaces::HexRecordType::ExtendedSegmentAddress == recordType) || (Interfaces::HexRecordType::StartSegmentAddress == recordType)) {
                    // 8 bit -> 16 bit
                    m_state = HexReaderState::Mode16;
                } else if ((Interfaces::HexRecordType::ExtendedLinearAddress == recordType) || (Interfaces::HexRecordType::StartLinearAddress == recordType)) {
                    // 8bit -> 32 bit
                    m_state = HexReaderState::Mode32;
                }
                break;
            case HexReaderState::Mode16:
                if ((Interfaces::HexRecordType::ExtendedLinearAddress == recordType) || (Interfaces::HexRecordType::StartLinearAddress == recordType)) {
                    // 16 bit -> 32 bit. NOT ALLOWED!
                    qCritical() << QObject::tr("Extended Linear Address or Start Linear Address in 16 bit mode.");
                    delete record;
                    throw QException();
                }
                break;
            case HexReaderState::Mode32:
                if ((Interfaces::HexRecordType::ExtendedSegmentAddress == recordType) || (Interfaces::HexRecordType::StartSegmentAddress == recordType)) {
                    // 32 bit -> 16 bit. NOT ALLOWED!
                    qCritical() << QObject::tr("Extended Segment Address or Start Segment Address in 32 bit mode.");
                    delete record;
                    throw QException();
                }
                break;
            default:;
            }

            // Processing record
            auto recordPayload = record->getData();
            auto recordOffset = record->getOffset();

            switch (recordType) {

            case Interfaces::HexRecordType::Data:
                // Adding data
                quint32 effectiveAddress;

                for (int index = 0; index < recordPayload.size(); index++) {
                    switch (m_state) {

                    case HexReaderState::Mode8:
                        effectiveAddress = index + recordOffset;
                        break;

                    case HexReaderState::Mode16:
                        effectiveAddress = m_baseAddress + (quint16)(index + recordOffset);
                        break;

                    case HexReaderState::Mode32:
                        effectiveAddress = m_baseAddress + index + recordOffset;
                        break;

                    default:;
                    }

                    result->Data.insert(effectiveAddress, recordPayload[index]);
                }
                break;

            case Interfaces::HexRecordType::ExtendedSegmentAddress:
                // Updating base address (16 bit mode)
                m_baseAddress = (recordPayload[USBAHIndex] << USBAHShift) | (recordPayload[USBALIndex] << USBALShift); // Lower bits 0-3 is 0x00
                break;

            case Interfaces::HexRecordType::ExtendedLinearAddress:
                // Updating base address (32 bit mode)
                m_baseAddress = (recordPayload[ULBAHIndex] << ULBAHShift) | (recordPayload[ULBALIndex] << ULBALShift); // Payload contains two upper bytes, two lower is 0x00
                break;

            default:;
            }

            delete record;
        }
        catch (...) {
            qCritical() << QObject::tr("Error while processing line %1 : %2").arg(lineNumber).arg(readLine);

            file.close();
            delete result;
            throw;
        }

        lineNumber ++;
    }

    file.close();

    return result;
}

} //Hex



