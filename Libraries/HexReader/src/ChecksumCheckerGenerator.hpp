#ifndef CHECKSUMCHECKERGENERATOR_HPP
#define CHECKSUMCHECKERGENERATOR_HPP

#include "interfaces/IChecksumCheckerGenerator.hpp"


namespace	Hex
{
class ChecksumCheckerGenerator : public Interfaces::IChecksumCheckerGenerator
{
public:
    bool check(const QVector<quint8> &data) override;
    quint8 generate(const QVector<uint8_t> &data) override;

    ~ChecksumCheckerGenerator() {};
};

} //Hex

#endif // CHECKSUMCHECKERGENERATOR_HPP
