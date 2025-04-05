#ifndef HEX_GLOBAL_HPP
#define HEX_GLOBAL_HPP

#include <QtGlobal>

#if defined Q_OS_WIN32  || defined Q_OS_WIN64
#if defined MAKE_LIB
#define LIB_EXPORT Q_DECL_EXPORT
#else
#define LIB_EXPORT Q_DECL_IMPORT
#endif //MAKE_LIB
#endif //Q_OS_WIN32 || Q_OS_WIN64

#endif // HEX_GLOBAL_HPP
