#ifndef HEXTOSTRINGCODEC_HPP
#define HEXTOSTRINGCODEC_HPP

#include <QRegularExpression>
#include <QScopedPointer>

#include "interfaces/IHexToStringCodec.hpp"

namespace	Hex
{
class HexToStringCodec : public Interfaces::IHexToStringCodec
{
public:
    HexToStringCodec();

    QString code(quint8 data) override;
    quint8 decode(const QString &data) override;

    ~HexToStringCodec();

private:

    static const QString StringRegExp;  //!< Regexp, for what data in Decode() have to match

    QScopedPointer<QRegularExpression> m_stringRegExp; //!< Regexp matcher for StringRegExp

    /*!
     * \brief Вывод сообщения в консоль, если строка не декодруется
     * \param str
     */
    void SignalizeStringIsNotDecodeable(const QString &str);
};
} //Hex

#endif // HEXTOSTRINGCODEC_HPP
