#ifndef IHEXTOSTRINGCODEC_HPP
#define IHEXTOSTRINGCODEC_HPP

#include "../hex_global.hpp"

#include <QString>

namespace HEX
{

namespace Interfaces
{

/*!
 * \brief Интерфейс класса кодирования quint8 в QString и обратно
 */
class IHexToStringCodec
{
public:
    virtual QString encode(uint8_t data) const = 0;
    virtual uint8_t decode(const QString &data) const = 0;


    virtual ~IHexToStringCodec() {}
};

}
}

#endif // IHEXTOSTRINGCODEC_HPP
