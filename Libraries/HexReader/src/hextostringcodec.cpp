#include "hextostringcodec.hpp"

#include <QDebug>
#include <QException>

namespace HEX
{

const QString HexToStringCodec::stringRegExp = "[0-9ABCDEF]{2}";

HexToStringCodec::HexToStringCodec()
{
    m_stringRegExp.reset(new QRegExp(stringRegExp));
}

QString HexToStringCodec::encode(uint8_t data) const
{
    return QString::number(data, 16).toUpper();
}

uint8_t HexToStringCodec::decode(const QString &data) const
{
    if (!m_stringRegExp->exactMatch(data)) {
        signalizeStringIsNotDecodeable(data);
    }

    auto isSuccess = false;
    quint8 result =static_cast<quint8>(data.toUInt(&isSuccess, 16));

    if (!isSuccess) {
        signalizeStringIsNotDecodeable(data);
    }

    return result;
}

void HexToStringCodec::signalizeStringIsNotDecodeable(const QString &str) const
{
    qCritical() << QObject::tr("Invalid string to decode: %1").arg(str);

    throw QException();
}

} //HEX
