#include "stdafx.h"
#include "Utils.h"

LPCTSTR PortTypeStrs[] = {
	TEXT("Unknown"), 
	TEXT("COM"), 
	TEXT("FT"), 
	TEXT("IP"), 
	TEXT("IPS")
};
LPCTSTR CvtTypeStrs[] = {
	TEXT("Unknown"), 
	TEXT("Z-397"), 
	TEXT("Z-397 Guard"), 
	TEXT("Z-397 IP"), 
	TEXT("Z-397 Web"),
	TEXT("Z5R Web"),
	TEXT("Matrix II Wi-Fi")
};

LPCTSTR CtrTypeStrs[] = {
	TEXT(""),
	TEXT("Gate 2000"),
	TEXT("Matrix-II Net"),
	TEXT("Matrix-III Net"),
	TEXT("Z-5R Net"),
	TEXT("Z-5R Net 8000"),
	TEXT("Guard-Net"),
	TEXT("Z-9 EHT Net"),
	TEXT("EuroLock EHT net"),
	TEXT("Z-5R Web"),
	TEXT("Matrix-II Wi-Fi"),
	TEXT("Matrix-VI NFC Net 8k"),
	TEXT("Matrix-VI EHK Net 2k"),
	TEXT("Matrix-VI Wi-Fi"),
	TEXT("Z-5R Web BT"),
	TEXT("Z-5R Wi-Fi"),
	TEXT("Matrix-II EH Web"),
	TEXT("Matrix-VI EH Web"),
	TEXT("Z-5R Web mini"),
	TEXT("Z-5R Net 16k")
};
LPCTSTR GuardModeStrs[] = {
	TEXT("Unknown"), 
	TEXT("Normal"), 
	TEXT("Advanced"), 
	TEXT("Test"), 
	TEXT("Accept")
};
LPCTSTR KeyModeStrs[] = {
	TEXT("Touch Memory"),
	TEXT("Proximity")
};
LPCTSTR KeyTypeStrs[] = {
	TEXT(""),
	TEXT("Normal"),
	TEXT("Blocking"),
	TEXT("Master")
};


BOOL CheckZGError(HRESULT nStatus, LPCTSTR pszContext)
{
	if (SUCCEEDED(nStatus))
		return TRUE;
	LPTSTR pBuffer = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | ((HRESULT_FACILITY(nStatus) == 4) ? FORMAT_MESSAGE_FROM_HMODULE : FORMAT_MESSAGE_FROM_SYSTEM),
		GetModuleHandle(_T("ZGuard.dll")), 
		(DWORD)nStatus,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&pBuffer, 
		0, 
		NULL);
	if (pBuffer != NULL)
	{
		_tprintf(TEXT("%s fail: %s\n"), pszContext, pBuffer);
		LocalFree(pBuffer);
	}
	getchar();
	return FALSE;
}

std::wstring ZKeyNumToStr(const Z_KEYNUM& rNum, BOOL fProximity)
{
	std::wstring s;

	if (fProximity)
	{
		int n;
		s.resize(128);
		n = _stprintf_s(&s[0], s.size(), TEXT("%d,%d"), rNum[3], *(PWORD)&rNum[1]);
		s.resize(n);
	}
	else
	{
		INT i, j;
		s.resize(rNum[0] * 2 + 1);
		j = 0;
		for (i = rNum[0]; i > 0; i--) {
			_stprintf_s(&s[j], s.size() - j, TEXT("%.2X"), rNum[i]);
			j += 2;
		}
		s.resize(rNum[0] * 2);
	}
	return s;
}


CZGuardLoader::CZGuardLoader()
{
    m_nStatus = ZG_Initialize(ZP_IF_NO_MSG_LOOP);
}

CZGuardLoader::~CZGuardLoader()
{
	if (SUCCEEDED(m_nStatus))
        ZG_Finalyze();
}
