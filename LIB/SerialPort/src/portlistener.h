#ifndef PORTLISTENER_H
#define PORTLISTENER_H


#define _write			0
#define _write_param	1
#define _read			2

#define err_transaction 1

#define MAX_SCAN_DEVICES 10

#include <QThread>
#include <QDebug>

#include "QSerialPort.h"
#include "XBeeTypes.h"

class PortListener : public QThread
{
    Q_OBJECT
public:
    explicit PortListener(QObject *parent = 0);
    void run();
    
signals:
    void initPort(QString);
    void portConnected(QSerialPort *port);
    void receiveFrame(char* frame, int size);

private:
    QSerialPort *m_port;
    byte com_AT[32];
    byte Receive_frame[128];
    byte Transmit_frame[128];
    byte Serial_numberH[8];
    byte Serial_numberL[8];

    char buffer_rcv[1024];
    byte sch_frame;
    char buf_frame[128];
    byte APIresponse;
    byte error_XB;
    byte sch_inf;
    word32 Source_addressL;
    word32 Source_addressH;

    int Find_portXB();
    bool Init_XB();
    byte Send_ATcommand(byte param, byte length, byte *adr);
    void receiveData();
    byte* Receive_APIframe (byte *address, byte cnt);

    
public slots:
    
};

#endif // PORTLISTENER_H
