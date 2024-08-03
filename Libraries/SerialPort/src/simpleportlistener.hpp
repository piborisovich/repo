/**
 * @file simpleportlistener.hpp
 * @brief Объявления простого потока для работы с последовательным портом
 */

#ifndef SIMPLEPORTLISTENER_HPP
#define SIMPLEPORTLISTENER_HPP

#include "portlistener.hpp"

/*!
 * \brief Простой поток для работы с последовательным портом (без реализации протокола)
 */
#if defined Q_OS_WIN32
class LIB_EXPORT SimplePortListener : public PortListener
#else
class SimplePortListener : public PortListener
#endif
{
    Q_OBJECT
public:
    explicit SimplePortListener(const PortConfig &portConfig = {1, PortConfig::BR_9600, PortConfig::ONE_STOPBIT}, QObject *parent = 0);

signals:
    void received(QByteArray data);

public slots:
    void onDataSend(QByteArray data);

protected slots:
    void on_received(char *data, int size);
};

#endif // SIMPLEPORTLISTENER_HPP
