#include "checksumcheckergenerator.hpp"

namespace HEX
{
bool ChecksumCheckerGenerator::check(QVector<uint8_t> data) const
{
    uint8_t buffer = 0x00; // Intentionally 8-bit, overflow is what we need.

    auto iterator = data.begin();
    while (iterator != data.end())
    {
        buffer += *iterator;
        iterator ++;
    }

    return 0x00 == buffer;
}

uint8_t ChecksumCheckerGenerator::generate(QVector<uint8_t> data) const
{
    int sum = std::accumulate(data.begin(), data.end(), 0, std::plus<uint8_t>());
    return 0xFF - static_cast<quint8>(sum) + 1;
}

} //HEX
