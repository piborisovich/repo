#ifndef PORTLISTENER_HPP
#define PORTLISTENER_HPP

#include <QThread>

#include "serialport.hpp"

class PortListener : public QThread
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
