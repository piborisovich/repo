#ifndef TOOLLIST_HPP
#define TOOLLIST_HPP

#include "itool.hpp"

#include <QList>

#include <memory.h>

using ToolList = QList< std::shared_ptr<ITool> >;

#endif // TOOLLIST_HPP
