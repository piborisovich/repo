#ifndef HEXREADER_HPP
#define HEXREADER_HPP

#include <QtGlobal>
#include <QString>

#include "hex_global.hpp"
#include "interfaces/IHexReader.hpp"

namespace Hex
{
/**
		 * @brief Possible Intel HEX reader states.
		 */
enum HexReaderState
{
    Mode8,
    Mode16,
    Mode32,
    End
};

/*!
 * \brief Класс ридера Hex
 */
class LIB_EXPORT HexReader : public Interfaces::IHexReader
{
public:
    HexReadonlyDataset* readFile(const QString &fileName) override;

    ~HexReader() {};

private:

    static const int USBAHIndex;     //!< Payload byte index for USBA upper byte (12-19)
    static const quint32 USBAHShift; //!< Shift USBAH to this value left
    static const int USBALIndex;     //!< Payload byte index for USBA lower byte (4-11)
    static const quint32 USBALShift; //!< Shift USBAL to this value left
    static const int ULBAHIndex;     //!< Payload byte index for ULBA upper byte (24-31)
    static const quint32 ULBAHShift; //!< Shift ULBAH to this value left to get 3th (from 0) byte of ULBA
    static const int ULBALIndex;     //!< Payload byte index for ULBA lower byte (16-23)
    static const quint32 ULBALShift; //!< Shift ULBAL to this value left to get 2th (from 0) byte of ULBA


    HexReaderState m_state; //!< State machine state
    quint32 m_baseAddress;  //!< Base address (meaning depends on _state)
};

} //Hex

#endif // HEXREADER_HPP
