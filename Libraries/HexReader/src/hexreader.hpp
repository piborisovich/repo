#ifndef HEXREADER_HPP
#define HEXREADER_HPP

#include "interfaces/ihexreader.hpp"

namespace HEX
{

/*!
 * \brief Информация о канале
 */
#ifdef LIB_EXPORT
class LIB_EXPORT HexReader : public Interfaces::IHexReader
#else
class HexReader : public Interfaces::IHexReader
#endif
{
    Q_OBJECT
public:
    HexReader();
    virtual ~HexReader() override {}

    std::shared_ptr<HexReadonlyDataset> readFile(const QString &fileName, int alignment = Alignment_32) override;

    /*!
     * \brief Возможные состояния
     */
    enum HexReaderState
    {
        Mode8,
        Mode16,
        Mode32,
        End
    };

    enum HexReaderAlignmentMode
    {
        Alignment_None,
        Alignment_16 = 2,
        Alignment_32 = 4
    };

private:
    static const int USBAHIndex;      //!< Индекс байта данных для старшего байта USBA (12-19)
    static const uint32_t USBAHShift; //!< Сдвиг USBAH влево
    static const int USBALIndex;      //!< Индекс байта данных для младшего байта USBA (4-11)
    static const uint32_t USBALShift; //!< Сдвиг USBAL влево
    static const int ULBAHIndex;      //!< Индекс байта данных для старшего байта ULBA (24-31)
    static const uint32_t ULBAHShift; //!< Сдвиг ULBAH влево
    static const int ULBALIndex;      //!< Индекс байта данных для младшего байта ULBA (16-23)
    static const uint32_t ULBALShift; //!< Сдвиг ULBAL влево

    uint32_t m_baseAddress;  //!< Базовый адрес
    HexReaderState m_state; //!< Состояние
};


} //HEX

#endif //HEXREADER_HPP
