#ifndef LOAD_CONTEXT_EXCEPTION_HPP
#define LOAD_CONTEXT_EXCEPTION_HPP

#include "../common.hpp"
#include "../exception.hpp"

namespace Core
{

namespace ApplicationContext
{

/*!
 * \brief Класс исключения по ошибке загрузки контекста
 */
#ifdef LIB_EXPORT
class LIB_EXPORT load_context_exception : public Exception
#else
class load_context_exception : public Exception
#endif
{
public:
    load_context_exception(std::string &&message) : Exception( std::forward<std::string>(message) ) { }
};


} //ApplicationContext
} //Core

#endif // LOAD_CONTEXT_EXCEPTION_HPP
