#ifndef CHECKSUMCHECKERGENERATOR_HPP
#define CHECKSUMCHECKERGENERATOR_HPP

#include "interfaces/ichecksumcheckergenerator.hpp"

#include <QVector>

namespace HEX
{

/*!
 * \brief Класс проверки и расчёта контрольной суммы
 */
#ifdef LIB_EXPORT
class LIB_EXPORT ChecksumCheckerGenerator : public Interfaces::IChecksumCheckerGenerator
#else
class ChecksumCheckerGenerator : public Interfaces::IChecksumCheckerGenerator
#endif
{
public:
    bool check(QVector<uint8_t> data) const override;
    uint8_t generate(QVector<uint8_t> data) const override;

    virtual ~ChecksumCheckerGenerator() override {}
};

} //HEX

#endif // CHECKSUMCHECKERGENERATOR_HPP
