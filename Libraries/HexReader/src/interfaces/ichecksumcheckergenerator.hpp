#ifndef ICHECKSUMCHECKERGENERATOR_HPP
#define ICHECKSUMCHECKERGENERATOR_HPP

#include "../hex_global.hpp"

#include <QVector>

namespace HEX
{
namespace Interfaces
{
/*!
 * \brief Интерфейс класса проверки и расчёта контрольной суммы
 */
class IChecksumCheckerGenerator
{
public:
    virtual bool check(QVector<uint8_t> data) const = 0;
    virtual uint8_t generate(QVector<uint8_t> data) const = 0;
    virtual ~IChecksumCheckerGenerator() {}
};
}
} //HEX

#endif // ICHECKSUMCHECKERGENERATOR_HPP
