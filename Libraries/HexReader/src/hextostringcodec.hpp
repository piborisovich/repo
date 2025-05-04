#ifndef HEXTOSTRINGCODEC_HPP
#define HEXTOSTRINGCODEC_HPP

#include "interfaces/ihextostringcodec.hpp"

#include <QRegularExpression>

#include <memory>

namespace HEX
{

/*!
 * \brief Класс кодирования quint8 в QString и обратно
 */
#ifdef LIB_EXPORT
class LIB_EXPORT HexToStringCodec : public Interfaces::IHexToStringCodec
#else
class HexToStringCodec : public Interfaces::IHexToStringCodec
#endif
{
public:
    HexToStringCodec();

    QString encode(uint8_t data) const override;
    uint8_t decode(const QString &data) const override;

    virtual ~HexToStringCodec() override {}

private:
    static const QString stringRegExp;      //!< Регулярное выражение для данных
    std::unique_ptr<QRegularExpression> m_stringRegExp; //!<

    /*!
     * \brief Сгенерировать исключение, если строка некорректна
     * \param str - Строка
     */
[[noreturn]] void signalizeStringIsNotDecodeable(const QString &str) const;

};
} //HEX

#endif // HEXTOSTRINGCODEC_HPP
