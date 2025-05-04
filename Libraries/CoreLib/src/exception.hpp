#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

namespace Core
{

/*!
 * \brief Класс исключения с сообщением
 */
class Exception : public std::exception
{
public:
    explicit Exception(std::string &&message) :  m_message( std::forward<std::string>(message) ) { }

    const char* what() const noexcept override
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};



} //Core

#endif // EXCEPTION_HPP
