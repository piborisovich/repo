#ifndef SIMPLECONTEXT_HPP
#define SIMPLECONTEXT_HPP

#include "../common.hpp"

#include <QDir>
#include <QVariant>

namespace Core
{

namespace ApplicationContext
{

/*!
 * \brief Данные приложения (простой контекст с сохранением в тектовый файл)
 */
#ifdef LIB_EXPORT
class LIB_EXPORT SimpleContext
#else
class SimpleContext
#endif
{
public:

    virtual ~SimpleContext();

    struct Checker;
    struct DirExistChecker;
    struct FileExistChecker;

    void load();
    void save();

    QVariant data(int field_id) noexcept;
    void setData(int field_id, const QVariant &data);

    bool isValid() const noexcept;

    void setContext_path(const QString &value);

    virtual void init() = 0;

protected:

    template<typename T, typename DataChecker>
    void addField(int id, T &&data)
    {
        m_fields.insert( std::make_pair(id, qMakePair( std::forward<T>(data), DataChecker() ) ) );
    }

private:
    std::map< int, QPair<QVariant, std::function<bool(QVariant)> > > m_fields; //!< Поля данных контекста (id : QPair<Данные, Предикат проверки>)
    bool m_isValid;        //!< Валидность
    QString m_contextPath; //!< Путь к файлу данных
};

#ifdef LIB_EXPORT
struct LIB_EXPORT SimpleContext::Checker
#else
struct SimpleContext::Checker
#endif
{
    bool operator()(const QVariant &data) const noexcept
    {
        QString str_data = data.toString();
        return str_data.isEmpty() ? false : true;
    }
};

#ifdef LIB_EXPORT
struct LIB_EXPORT SimpleContext::DirExistChecker
#else
struct SimpleContext::DirExistChecker
#endif
{
bool operator()(const QVariant &path) const noexcept
{
    QString str_path = path.toString();
    return !str_path.isEmpty() && QDir(str_path).exists();
}

};

#ifdef LIB_EXPORT
struct LIB_EXPORT SimpleContext::FileExistChecker
#else
struct SimpleContext::FileExistChecker
#endif
{
bool operator()(const QVariant &path) const noexcept
{
    QString str_path = path.toString();
    return !str_path.isEmpty() && QFile(path.toString()).exists();
}

};

} //ApplicationContext
} //Core

#endif // SIMPLECONTEXT_HPP
