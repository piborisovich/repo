#pragma once

#include <QtGlobal>

#ifndef MAKE_STATIC_LIB
#  ifdef Q_OS_WIN32
#    ifdef MAKE_LIB
#      define LIB_EXPORT Q_DECL_EXPORT
#    else
#      define LIB_EXPORT Q_DECL_IMPORT
#    endif //MAKE_LIB
#  endif //Q_OS_WIN32
#endif //MAKE_STATIC_LIB


#ifndef MAKE_STATIC_LIB
#  ifdef BUILD_WIDGETS_LIB
#    define WIDGETS_EXPORT Q_DECL_EXPORT
#  else
#    define WIDGETS_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define WIDGETS_EXPORT
#endif

