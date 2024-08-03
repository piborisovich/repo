/**
 * @file portlistener.hpp
 * @brief Работа с последовательным портом в потоке
 */

#ifndef PORTLISTENER_HPP
#define PORTLISTENER_HPP

#include "serialport.hpp"

#include <QThread>

#if defined Q_OS_WIN32  || defined Q_OS_WIN64
  #if defined MAKE_LIB
    #define LIB_EXPORT Q_DECL_EXPORT
  #else
    #define LIB_EXPORT Q_DECL_IMPORT
  #endif //MAKE_LIB
#endif //Q_OS_WIN32 || Q_OS_WIN64

#if defined Q_OS_WIN32
class LIB_EXPORT PortListener : public QThread
#else
class PortListener : public QThread
#endif
{
    Q_OBJECT
public:
    explicit PortListener(const PortConfig &portConfig = {1, PortConfig::BR_9600, PortConfig::ONE_STOPBIT}, QObject *parent = 0);
    virtual ~PortListener() override;

    void run() override;

public slots:
    void on_portDisconnect();
    
signals:
    void portConnected(bool connected);

protected:
    void sendData(const QByteArray &data);

protected slots:
    virtual void on_received(char *data, int size) = 0;

private:
    SerialPort *m_port;
    bool m_stopped;
    char *m_buffer_rcv;
};

#endif // PORTLISTENER_HPP
