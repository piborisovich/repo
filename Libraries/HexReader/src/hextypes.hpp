#ifndef HEXTYPES_HPP
#define HEXTYPES_HPP

namespace Hex
{
/*!
 * \brief Возможные режимы Intel Hex. Режим определяет допустимый набор записей
 */
enum class HexAddressingMode
{
    Mode8bit,
    Mode16bit,
    Mode32bit,
};
} //Hex

#endif // HEXTYPES_HPP
