#include "ChecksumCheckerGenerator.hpp"

namespace Hex
{
bool ChecksumCheckerGenerator::check(const QVector<quint8> &data)
{
    quint8 buffer = 0x00; // Intentionally 8-bit, overflow is what we need.

    auto iterator = data.begin();
    while (iterator != data.end()) {
        buffer += *iterator;
        iterator ++;
    }

    return 0x00 == buffer;
}

quint8 ChecksumCheckerGenerator::generate(const QVector<quint8> &data)
{
    return 0x100 - std::accumulate(data.begin(), data.end(), 0, std::plus<quint8>());
}

} //Hex
