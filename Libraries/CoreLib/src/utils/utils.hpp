#pragma once

#include <QObject>

#include <type_traits>

namespace Core
{

namespace Utils
{

template<typename Func1, typename Func2>
inline void secure_connect(const typename QtPrivate::FunctionPointer<Func1>::Object *sender, Func1 signal,
                           const typename QtPrivate::FunctionPointer<Func2>::Object *receiver, Func2 slot,
                           Qt::ConnectionType type = Qt::AutoConnection )
{
    auto result = QObject::connect(sender, signal, receiver, slot, type);
    Q_ASSERT(result);
}

template <typename Func1, typename Func2>
inline void secure_connect(const typename QtPrivate::FunctionPointer<Func1>::Object *sender, Func1 signal, Func2 slot)
{
    auto result = QObject::connect(sender, signal, slot);
    Q_ASSERT(result);
}

} //Utils
} //Core
