#ifndef IHEXTOSTRINGCODEC_HPP
#define IHEXTOSTRINGCODEC_HPP

#include <QString>

namespace	Hex
{
namespace Interfaces
{

/*!
 * \brief Интерфейс кодирования байта в троку и обратно
 */
class IHexToStringCodec
{
public:

    virtual QString code(quint8 data) = 0;

    virtual uint8_t decode(const QString &data) = 0;

    virtual ~IHexToStringCodec() {}
};
}
} //Hex

#endif // IHEXTOSTRINGCODEC_HPP
