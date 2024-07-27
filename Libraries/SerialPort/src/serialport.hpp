/**
 * @file serialport.hpp
 * @brief Работа с последовательным портом
 */

#ifndef SERIALPORT_HPP
#define SERIALPORT_HPP

#include <QObject>

#if defined (Q_OS_WIN32)
#include <windows.h>
#endif

/*!
 * \brief Конфигурация последовательного порта
 */
struct PortConfig
{
    enum BAUD_RATES
    {
        BR_110 = 110,
        BR_300 = 300,
        BR_600 = 600,
        BR_1200 = 1200,
        BR_2400 = 2400,
        BR_4800 = 4800,
        BR_9600 = 9600,
        BR_14400 = 14400,
        BR_19200 = 19200,
        BR_38400 = 38400,
        BR_56000 = 56000,
        BR_57600 = 57600,
        BR_115200 = 115200,
        BR_128000 = 128000,
        BR_256000 = 256000
    };

    int portnum;
    BAUD_RATES baudrate;
};

/*!
 * \brief Базовый функционал последовательного порта
 */
class SerialPort : public QObject
{
public:
    SerialPort(const PortConfig &portConfig);
    virtual ~SerialPort();
public:
    bool open(int portnum,int baudrate);
    bool openUsb(int portnum,int baudrate);
    bool close();
    bool isOpened();
    bool initComPort(int baudrate);


    bool write(const char* buf,int cnt);
    bool writeln(const char* buf,int cnt);
    bool read(char* buf, int bufsz, int* cnt);

protected:

#if defined (Q_OS_WIN32)
    HANDLE m_handle;

#elif defined (Q_OS_LINUX)
    int    m_handle;
#endif

private:
    SerialPort(const SerialPort &serialPort) = delete;
    SerialPort& operator=(const SerialPort&) = delete;
};

#endif // SERIALPORT_HPP
