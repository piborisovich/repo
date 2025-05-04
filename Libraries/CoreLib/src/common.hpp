#ifndef COMMON_HPP
#define COMMON_HPP

#include<QtCore>

#ifndef MAKE_STATIC_LIB

#ifdef Q_OS_WIN32

#ifdef MAKE_LIB
#define LIB_EXPORT Q_DECL_EXPORT
#else
#define LIB_EXPORT Q_DECL_IMPORT
#endif //MAKE_LIB

#endif //Q_OS_WIN32

#endif //MAKE_STATIC_LIB

#endif // COMMON_HPP
