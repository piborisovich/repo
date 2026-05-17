#pragma once
#include <Windows.h>
#include <string>
#include "ZBase.h"
#include "ZGuard.h"

//#define GetBit(val, bitN)	(BOOL)((val >> (bitN)) & 1)

extern LPCTSTR PortTypeStrs[5];
extern LPCTSTR CvtTypeStrs[7];
extern LPCTSTR CtrTypeStrs[20];
extern LPCTSTR GuardModeStrs[5];
extern LPCTSTR KeyModeStrs[2];
extern LPCTSTR KeyTypeStrs[4];

BOOL CheckZGError(HRESULT nStatus, LPCTSTR pszContext);
std::wstring ZKeyNumToStr(const Z_KEYNUM& rNum, BOOL fProximity);

/*!
 * \brief Загрузчик библиотеки ZGuard
 */
class CZGuardLoader
{
public:
	HRESULT m_nStatus;
public:
	CZGuardLoader();
	virtual ~CZGuardLoader();

	inline BOOL IsLoaded() const { return SUCCEEDED(m_nStatus); }
};
