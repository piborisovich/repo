#pragma once

#include <type_traits>

namespace Core
{
namespace Types
{
namespace Utils
{

template <typename E>
constexpr typename std::underlying_type<E>::type toUType(E enumerator) noexcept
{
    return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

} //Utils
} //Types
} //Core
