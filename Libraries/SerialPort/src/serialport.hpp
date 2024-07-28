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

    enum STOP_BITS
    {
        ONE_STOPBIT = 0,
        ONE5_STOPBITS,
        TWO_STOPBITS
    };

    int portnum;
    BAUD_RATES baudrate;
    STOP_BITS stopBits;
};

/*!
 * \brief Базовый функционал последовательного порта
 */
class SerialPort : public QObject
{
public:
    SerialPort(const PortConfig &portConfig = {1, PortConfig::BR_9600, PortConfig::ONE_STOPBIT});
    virtual ~SerialPort();
public:
    bool open();
    bool openUsb();
    bool close();
    bool isOpened();

    bool write(const char* buf,int cnt);
    bool writeln(const char* buf,int cnt);
    bool read(char* buf, int bufsz, int* cnt);

    PortConfig portConfig() const;
    void setPortConfig(const PortConfig &newPortConfig);

protected:

#if defined (Q_OS_WIN32)
    HANDLE m_handle;

#elif defined (Q_OS_LINUX)
    int    m_handle;
#endif

private:

    static const int TX_BUF_SIZE; //!< Размер приемного буфера
    static const int RX_BUF_SIZE; //!< Размер буфера передатчика
    bool initComPort();

    SerialPort(const SerialPort &serialPort) = delete;
    SerialPort& operator=(const SerialPort&) = delete;

    PortConfig m_portConfig;
};

#endif // SERIALPORT_HPP
