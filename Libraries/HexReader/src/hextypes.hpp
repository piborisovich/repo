#ifndef HEXTYPES_HPP
#define HEXTYPES_HPP

#include "hex_global.hpp"

namespace HEX
{
/*!
 * \brief Режим Intel Hex
 */
enum class HexAddressingMode
{
    Mode8bit,
    Mode16bit,
    Mode32bit,
};

} //HEXLib

#endif //HEXTYPES_HPP
