#ifndef IHEXREADER_HPP
#define IHEXREADER_HPP

#include "../hexreadonlydataset.hpp"
#include "../hexrecord.hpp"

#include <QString>
#include <QObject>

#include <memory>

namespace HEX
{

namespace Interfaces
{
/*!
 * \brief Интерфейс класса чтения Hex
 */
#ifdef LIB_EXPORT
class LIB_EXPORT IHexReader : public QObject
#else
class IHexReader : public QObject
#endif
{
    Q_OBJECT
public:
    IHexReader() : QObject(nullptr) {}

    /*!
     * \brief Чтение данных из файла
     * \param fileName - Имя файла
     * \param alignment - Выравнивание данных
     * \return
     */
    virtual std::shared_ptr<HexReadonlyDataset> readFile(const QString &fileName, int alignment) = 0;

    virtual ~IHexReader() {}
};

} //Interfaces
} //HEX

#endif // IHEXREADER_HPP
