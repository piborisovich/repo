#ifndef PORTLISTENER_HPP
#define PORTLISTENER_HPP

#include <QThread>

#include "serialport.hpp"

class PortListener : public QThread
{
    Q_OBJECT
public:
    explicit PortListener(QObject *parent = 0);
    void run();
    
signals:
    void initPort(QString);
    void portConnected(bool connected);
    void receiveFrame(char* frame, int size);

private:
    SerialPort *m_port;

    //char buffer_rcv[1024];
   // byte sch_frame;
    //char buf_frame[128];
};

#endif // PORTLISTENER_HPP
