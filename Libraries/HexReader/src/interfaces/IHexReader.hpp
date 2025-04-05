#ifndef IHEXREADER_HPP
#define IHEXREADER_HPP

#include <QString>

#include "../hex_global.hpp"
#include "../HexReadonlyDataset.hpp"

namespace Hex
{
namespace Interfaces
{
/*!
 * \brief Интерфейс Hex ридера
 */
class LIB_EXPORT IHexReader
{
public:

    virtual HexReadonlyDataset* readFile(const QString &fileName) = 0;

    virtual ~IHexReader() { }
};
}
} //Hex

#endif // IHEXREADER_HPP
