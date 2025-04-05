#include"HexToStringCodec.hpp"

#include <QDebug>
#include <QException>

namespace Hex
{

const QString HexToStringCodec::StringRegExp = "[0-9ABCDEF]{2}";

HexToStringCodec::HexToStringCodec()
{
    m_stringRegExp.reset( new QRegularExpression(StringRegExp) );
}

QString HexToStringCodec::code(quint8 data)
{
    return QString::number(data, 16).toUpper();
}

quint8 HexToStringCodec::decode(const QString &data)
{
    if (!m_stringRegExp->match(data).hasMatch()) {
        SignalizeStringIsNotDecodeable(data);
    }

    auto isSuccess = false;
    quint8 result = quint8(data.toUInt(&isSuccess, 16));

    if (!isSuccess) {
        SignalizeStringIsNotDecodeable(data);
    }

    return result;
}

void HexToStringCodec::SignalizeStringIsNotDecodeable(const QString &str)
{
    qCritical() << QObject::tr("Invalid string to decode: %1").arg(str);

    throw new QException();
}

HexToStringCodec::~HexToStringCodec()
{
}

} //Hex
