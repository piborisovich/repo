// QSerialPort.h: interface for the QSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QSERIALPORT_H__82D8D45A_746C_4ADF_93D9_3F0C9DA8C426__INCLUDED_)
#define AFX_QSERIALPORT_H__82D8D45A_746C_4ADF_93D9_3F0C9DA8C426__INCLUDED_

#include <qobject.h>
#include <stdlib.h>
#include <stdio.h>

#if defined (Q_OS_WIN32)
#include "windows.h"
#endif
/////////////////////////////////////////////////////////////////////
//
//  -
//
//	: Windows (win32)/Linux
//
//  (C)  .. 2008
//
/////////////////////////////////////////////////////////////////////

// Class Forward Declarations
class QTimer;

class QSerialPort : public QObject
{

public:
        QSerialPort();
        virtual ~QSerialPort();

protected:

//#ifdef _WINDOWS
#if defined (Q_OS_WIN32)
        HANDLE handle;      //

#elif defined (Q_OS_LINUX)
        int    handle;      //
#endif

public:
        // /
        //portnum - номер COM порта
        //baudrate - скорость обмена
        bool Open(int portnum,int baudrate);
        bool Openu(int portnum,int baudrate);
        bool Close();
        bool IsOpened();
        bool InitCOMPort(int baudrate);


        // /
        bool Write(const char* buf,int cnt);
        bool Writeln(const char* buf,int cnt);
        bool Read(char* buf, int bufsz, int* cnt);
};

#endif // !defined(AFX_QSERIALPORT_H__82D8D45A_746C_4ADF_93D9_3F0C9DA8C426__INCLUDED_)
