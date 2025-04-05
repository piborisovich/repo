#ifndef ICHECKSUMCHECKERWRITER_HPP
#define ICHECKSUMCHECKERWRITER_HPP

#include <QVector>

namespace Hex
{
namespace Interfaces
{
/*!
 * \brief Интерфейс для проверки и генерации контрольной суммы
 */
class IChecksumCheckerGenerator
{
public:

    virtual bool check(const QVector<quint8> &data) = 0;

    virtual quint8 generate(const QVector<quint8> &data) = 0;

    virtual ~IChecksumCheckerGenerator() {}
};
}
} //Hex

#endif // ICHECKSUMCHECKERWRITER_HPP
