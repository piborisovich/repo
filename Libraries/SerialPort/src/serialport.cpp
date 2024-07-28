#include "serialport.hpp"

#include <stdlib.h>
#include <stdio.h>

#if defined (Q_OS_WIN32)
#include "windows.h"
#include "tchar.h"

#elif defined (Q_OS_LINUX)

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/signal.h>

//    minicom
void m_setparms(int fd, char *baudr, char *par, char *bits, char *stopb,
                int hwf, int swf);

int get_device_status(int fd);

// Turn On/Off Hardware Workflow
void m_sethwf(int fd, int on);

// Set Ready To Send
void m_setrts(int fd);

#endif

const int SerialPort::TX_BUF_SIZE = 1024;
const int SerialPort::RX_BUF_SIZE = 1024;

SerialPort::SerialPort(const PortConfig &portConfig) :
    m_portConfig(portConfig)
{
//#ifdef _WINDOWS
#if defined (Q_OS_WIN32)
    m_handle = INVALID_HANDLE_VALUE;

#elif defined (Q_OS_LINUX)
    m_handle = -1;
#endif
}

SerialPort::~SerialPort()
{
}

bool SerialPort::open()
{
    if (isOpened()) close();

// Windows
#if defined (Q_OS_WIN32)
    char buf [100];
    sprintf(buf, "\\\\.\\COM%d", m_portConfig.portnum);

    m_handle =  CreateFileA(reinterpret_cast<char*>(buf),
                           GENERIC_READ | GENERIC_WRITE,
                           0,	// exclusive access
                           0,	// no security attrs
                           OPEN_EXISTING,
                           0,
                           nullptr);

    if (m_handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }
#elif defined (Q_OS_LINUX)

    //	Linux Version

    char buf [100];
    sprintf(buf, "/dev/ttyS%d", portnum - 1);

    // Open Device to be non blocking
    // O_NDELAY causes non blocking read
    m_handle = open(buf, O_RDWR|O_NOCTTY|O_NDELAY);
    if (handle < 0) return false;
#endif

    if ( !initComPort() ){
        return false;
    }
    return true;
}
bool SerialPort::openUsb()
{
    //Проверяем открыт ли порт. Если открыт закрываем.
    if (isOpened()){
        close();
    }

// Windows
#if defined (Q_OS_WIN32)

    char buf [100];
    sprintf(buf, "\\\\.\\COM%d", m_portConfig.portnum);

    m_handle =  CreateFileA(reinterpret_cast<char*>(buf),
                           GENERIC_READ | GENERIC_WRITE,
                           0,	// exclusive access
                           nullptr,	// no security attrs
                           OPEN_EXISTING,
                           0,
                           nullptr);

    if (m_handle == INVALID_HANDLE_VALUE){
        return false;
    }

#elif defined (Q_OS_LINUX)

    //	Linux Version

    char buf [100];
    sprintf(buf, "/dev/ttyUSB%d", portnum-1);
    //sprintf(buf, "/dev/ttyS%d", portnum-1);

    // Open Device to be non blocking
    // O_NDELAY causes non blocking read
    m_handle = open(buf, O_RDWR|O_NOCTTY|O_NDELAY);
    if (handle < 0)
        return false;
#endif

    if ( !initComPort() )
        return false;

    return true;
}
bool SerialPort::close()
{
#if defined (Q_OS_WIN32)

    if (m_handle == INVALID_HANDLE_VALUE) return false;
    CloseHandle(m_handle);
    m_handle = INVALID_HANDLE_VALUE;
    return true;

#elif defined (Q_OS_LINUX)

    if (m_handle < 0) return false;
    close(handle);
    handle = -1;
    return true;

#endif
}

bool SerialPort::isOpened()
{
#if defined (Q_OS_WIN32)
    return m_handle != INVALID_HANDLE_VALUE;

#elif defined (Q_OS_LINUX)
    return m_handle >= 0;
#endif
}

bool SerialPort::write(const char* buf,int cnt)
{
    if ( !isOpened() ) return false;

#if defined (Q_OS_WIN32)
    DWORD wrtn;

    if (WriteFile(m_handle, buf, cnt, &wrtn, nullptr) && (static_cast<DWORD>(cnt) == wrtn)) return true;

#elif defined (Q_OS_LINUX)
    if (write(m_handle, buf, cnt) == cnt)
    {
        //	qWarning(tr("wr cnt = %1").arg(cnt));//		qDebug(QString::number(cnt));
        return true;

    }
#endif
    return false;
}

bool SerialPort::writeln(const char* buf,int cnt)
{
    const char rn[] = "\r\n";
    return write(buf, cnt) && write(rn, 2);
}

bool SerialPort::read(char* buf, int bufsz, int* cnt)
{
    if (!isOpened()) return false;

#if defined (Q_OS_WIN32)

    if (ReadFile(m_handle, buf, bufsz, reinterpret_cast<DWORD*>(cnt), nullptr))
        return true;

    return false;

#elif defined (Q_OS_LINUX)
    *cnt = read(handle,buf,bufsz);

    //qWarning(tr("read cnt = %1").arg(*cnt));
    return true;
//	qWarning(tr("read cnt = %1").arg(*cnt));

#endif
}

PortConfig SerialPort::portConfig() const
{
    return m_portConfig;
}

void SerialPort::setPortConfig(const PortConfig &newPortConfig)
{
    m_portConfig = newPortConfig;
}


bool SerialPort::initComPort()
{

// Windows Implementation
#if defined (Q_OS_WIN32)

    DCB Dcb; // структура DCB
    COMMTIMEOUTS TimeOuts; // Структура COMMTIMEOUTS

    Dcb.DCBlength = sizeof(DCB); //Длина структуры DCB
    GetCommState(m_handle, &Dcb);

    if (m_portConfig.baudrate == 0){
        m_portConfig.baudrate = PortConfig::BR_9600;
    }

    Dcb.BaudRate = m_portConfig.baudrate;   //Скорость передачи данных
    Dcb.ByteSize = 8;                       //Число информационных бит
    Dcb.Parity =   NOPARITY;                //Контроль четности(NOPARITY - Бит четности отсутствует)   //EVENPARITY; //NOPARITY
    Dcb.fBinary =  TRUE;	                //binary mode must be true (Двоичный режим обмена. Всегда TRUE)
    Dcb.fParity =  FALSE;	                //enable parity checking (Включить контроль четности)
    Dcb.StopBits = m_portConfig.stopBits;//ONESTOPBIT;  //Число стоповых бит (ONESTOPBIT - один стоповый бит)
    Dcb.fOutxCtsFlow = FALSE;	            //CTS output flow control (Режим слежения за сигналом CTS)
    Dcb.fOutxDsrFlow = FALSE;	            //DSR output flow control (Режим слежения за сигналом DSR)
    Dcb.fDtrControl =  DTR_CONTROL_ENABLE;	//DTR flow control type (Использование линии DTR. DTR_CONTROL_ENABLE - Разрешить)
    Dcb.fDsrSensitivity   = FALSE;	        //DSR sensitivity
    Dcb.fTXContinueOnXoff = FALSE;	        //XOFF continues Tx
    Dcb.fOutX = FALSE;	                    //XON/XOFF out flow control
    Dcb.fInX  = FALSE;	                    //XON/XOFF in flow control
    Dcb.fErrorChar = FALSE;	                //enable error replacement
    Dcb.fNull = FALSE;	                    //enable null stripping
    Dcb.fRtsControl = RTS_CONTROL_ENABLE;	//RTS flow control (Режим управления потоком для сигнала RTS.
        //RTS_CONTROL_ENABLE - разрешить использование линии RTS)
    Dcb.fAbortOnError = FALSE;	            // abort reads/writes on error
    Dcb.XonLim =   FALSE;	                // transmit XON threshold
    Dcb.XoffLim =  FALSE;	                // transmit XOFF threshold
    Dcb.XonChar =  FALSE;	                // Tx and Rx XON character
    Dcb.XoffChar = FALSE;	                // Tx and Rx XOFF character

    if(!(SetCommState(m_handle, &Dcb))){
        return false;
    }

    if(!SetupComm(m_handle,
                   RX_BUF_SIZE,	//size of input buffer
                   TX_BUF_SIZE	//size of output buffer
                   )) {
        return false;
    }

    //discard of characters from input and output buffers
    //and reset all commications operations
    if(!PurgeComm(m_handle,
                  PURGE_TXABORT | PURGE_RXABORT |
                  PURGE_TXCLEAR | PURGE_RXCLEAR )){
        return false;
    }

    //Установка временных задержек
    TimeOuts.ReadIntervalTimeout = MAXDWORD;
    TimeOuts.ReadTotalTimeoutMultiplier = 0;
    TimeOuts.ReadTotalTimeoutConstant = 150;
    TimeOuts.WriteTotalTimeoutMultiplier = 0;
    TimeOuts.WriteTotalTimeoutConstant = 150;

    if (!SetCommTimeouts(m_handle, &TimeOuts)){
        return false;
    }

    if (!SetCommMask(m_handle, EV_RXCHAR | EV_TXEMPTY )){
        return false;
    }

    return true;

#elif defined (Q_OS_LINUX)
    // Linux Implementation
    char buf[20];
    sprintf(buf,"%d",baudrate);
    char bts = '8';
    char stopbts = '8';
    char par = 'N';
    char hasRTS = 'Y';  // Hardware Work Flow
    char hasXON = 0;    // Software Work Flow
    m_setparms(m_handle, buf, &par, &bts, &stopbts, hasRTS, hasXON);

    return true;
#endif
}

//   minicom
#if defined (Q_OS_LINUX)

// from minicom package
/*
 * Set baudrate, parity and number of bits.
 */


void m_setparms(int fd, char *baudr, char *par, char *bits, char *stopb,
                int hwf, int swf)
{
    int spd = -1;
    int newbaud;
    int bit = bits[0];

    struct termios tty;

    qWarning("Accessing serial port setup");
    tcgetattr(fd, &tty);


    /* We generate mark and space parity ourself. */
    if (bit == '7' && (par[0] == 'M' || par[0] == 'S')){
        bit = '8';
    }

    /* Check if 'baudr' is really a number */
    if ((newbaud = (atol(baudr) / 100)) == 0 && baudr[0] != '0'){
        newbaud = -1;
    }

    switch (newbaud) {
    case 0:spd = B0;break;
    case 3:	spd = B300;	break;
    case 6:	spd = B600;	break;
    case 12:	spd = B1200;	break;
    case 24:	spd = B2400;	break;
    case 48:	spd = B4800;	break;
    case 96:	spd = B9600;	break;
    case 192:	spd = B19200;	break;
    case 384:	spd = B38400;	break;
    case 576:	spd = B57600;	break;
    case 1152:	spd = B115200;	break;
    case 2304:	spd = B230400;	break;
    }

    qWarning("Setting baudrates");
    if (spd != -1){
        cfsetospeed(&tty, (speed_t)spd);
        cfsetispeed(&tty, (speed_t)spd);
    }

    qWarning("Setting bits");
    switch (bit) {
    case '5':
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS5;
        break;
    case '6':
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS6;
        break;
    case '7':
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS7;
        break;
    case '8':
    default:
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
        break;
    }
    /* Set into raw, no echo mode */
    tty.c_iflag = IGNPAR; //IGNBRK;
    qWarning("Chosing raw input mode");
    tty.c_lflag &= ~(ICANON|ECHO|ECHOE|ISIG); //0;
    tty.c_oflag = 0;
    tty.c_cflag |= CLOCAL | CREAD;
#ifdef _DCDFLOW
    tty.c_cflag &= ~CRTSCTS;
#endif
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    if (swf){
        tty.c_iflag |= IXON | IXOFF;
    } else {
        tty.c_iflag &= ~(IXON|IXOFF|IXANY);
    }
    tty.c_cflag &= ~(PARENB | PARODD);
    if (par[0] == 'E')
        tty.c_cflag |= PARENB;
    else if (par[0] == 'O')
        tty.c_cflag |= (PARENB | PARODD);

    if (stopb[0] == '2')
        tty.c_cflag |= CSTOPB;
    else
        tty.c_cflag &= ~CSTOPB;

    tcsetattr(fd, TCSANOW, &tty);

    m_setrts(fd);


#ifndef _DCDFLOW
    m_sethwf(fd, hwf);
#endif
}

int get_device_status(int fd)
{
    struct termios t;
    return !ioctl(fd, TCGETS, &t);
}

/* Set hardware flow control. */
void m_sethwf(int fd, int on)
{
#ifdef _DGUX_SOURCE
    struct termiox x;
#endif
#ifdef POSIX_TERMIOS
    struct termios tty;
#endif

#ifdef POSIX_TERMIOS
    tcgetattr(fd, &tty);
    if (on)
        tty.c_cflag |= CRTSCTS;
    else
        tty.c_cflag &= ~CRTSCTS;
    tcsetattr(fd, TCSANOW, &tty);
#endif

#ifdef _DGUX_SOURCE
    if (ioctl(fd, TCGETX, &x) < 0) {
        fprintf(stderr, _("can't get termiox attr.\n"));
        return;
    }
    x.x_hflag = on ? RTSXOFF|CTSXON : 0;

    if (ioctl(fd, TCSETX, &x) < 0) {
        fprintf(stderr, _("can't set termiox attr.\n"));
        return;
    }
#endif
}

/* Set RTS line. Sometimes dropped. Linux specific? */
void m_setrts(int fd)
{
#if defined(TIOCM_RTS) && defined(TIOCMODG)
    {
        int mcs=0;

        ioctl(fd, TIOCMODG, &mcs);
        mcs |= TIOCM_RTS;
        ioctl(fd, TIOCMODS, &mcs);
    }
#endif
#ifdef _COHERENT
    ioctl(fd, TIOCSRTS, 0);
#endif
}

#endif // ndef _WINDOWS
