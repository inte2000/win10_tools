#include "stdafx.h"
#include "WzRegKey.h"

#define WZ_ASSERT assert

CWzRegKey::CWzRegKey()
{
	m_hKey = NULL;
}

CWzRegKey::CWzRegKey(HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired)
{
	m_hKey = NULL;
	CreateKey(hKey,lpSubKey,samDesired);
}

CWzRegKey::~CWzRegKey()
{
	CloseKey();
}

LONG CWzRegKey::CreateKey(HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired)
{
	WZ_ASSERT(m_hKey == NULL);
	DWORD dwDisposition;

	LONG lRtn = ::RegCreateKeyEx(hKey,lpSubKey,0,NULL,REG_OPTION_NON_VOLATILE,samDesired,NULL,&m_hKey,&dwDisposition);
	if(lRtn != ERROR_SUCCESS)
		m_hKey = NULL;

	return lRtn;
}

LONG CWzRegKey::OpenKey(HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired)
{
	WZ_ASSERT(m_hKey == NULL);
	
	LONG lRtn = ::RegOpenKeyEx(hKey,lpSubKey,0,samDesired,&m_hKey);
	if(lRtn != ERROR_SUCCESS)
		m_hKey = NULL;

	return lRtn;
}

LONG CWzRegKey::CloseKey()
{
	LONG lRtn = ERROR_SUCCESS;
	if(m_hKey != NULL)
	{
		lRtn = ::RegCloseKey(m_hKey);
		m_hKey = NULL;
	}

	return lRtn;
}

LONG CWzRegKey::GetInteger(LPCTSTR lpszValueName, int& nValue, int nDefaultValue)
{
	WZ_ASSERT(m_hKey != NULL);

	DWORD dwValue;
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(int);
	LONG lRtn = ::RegQueryValueEx(m_hKey,lpszValueName,NULL,&dwType,(LPBYTE)&dwValue,&dwSize);
	if(lRtn == ERROR_SUCCESS)
		nValue = (int)dwValue;
	else
		nValue = nDefaultValue;
	
	return lRtn;
}

LONG CWzRegKey::GetDword(LPCTSTR lpszValueName, DWORD& dwValue, DWORD dwDefaultValue)
{
	WZ_ASSERT(m_hKey != NULL);
	
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(DWORD);
	LONG lRtn = ::RegQueryValueEx(m_hKey,lpszValueName,NULL,&dwType,(LPBYTE)&dwValue,&dwSize);
	if(lRtn != ERROR_SUCCESS)
		dwValue = dwDefaultValue;
	
	return lRtn;
}

LONG CWzRegKey::GetString(LPCTSTR lpszValueName, LPTSTR lpValueBuf, int nBufSize, LPCTSTR lpDefaultValue)
{
	WZ_ASSERT(m_hKey != NULL);
	WZ_ASSERT(lpValueBuf != NULL);

	DWORD dwType = REG_SZ;
	DWORD dwSize = nBufSize;
	LONG lRtn = ::RegQueryValueEx(m_hKey,lpszValueName,NULL,&dwType,(LPBYTE)lpValueBuf,&dwSize);
	if(lRtn != ERROR_SUCCESS)
	{
		if(lpDefaultValue != NULL)
		{
			int nDefLenth = lstrlen(lpDefaultValue);
			if(nDefLenth < nBufSize)
				lstrcpy(lpValueBuf,lpDefaultValue);
			else
			{
				lstrcpyn(lpValueBuf,lpDefaultValue,nBufSize);
				lpValueBuf[nBufSize] = 0;
			}
		}
		else
		{
			lpValueBuf[0] = 0;
		}
	}
	
	return lRtn;
}

LONG CWzRegKey::GetPoint(LPCTSTR lpszValueName, POINT& ptValue)
{
	WZ_ASSERT(m_hKey != NULL);

	TCHAR szBuf[128];
	LONG lRtn = GetString(lpszValueName,szBuf,128,NULL);
	if(lRtn == ERROR_SUCCESS)
	{
		int nField = _stscanf_s(szBuf,_T("POINT(%d,%d)"),&ptValue.x,&ptValue.y);
		if(nField != 2)
			lRtn = -1;
	}
	
	return lRtn;
}

LONG CWzRegKey::GetColor(LPCTSTR lpszValueName, COLORREF& crValue, COLORREF crDefaultValue)
{
	TCHAR szBuf[128];
	LONG lRtn = GetString(lpszValueName,szBuf,128,NULL);
	if(lRtn == ERROR_SUCCESS)
	{
		int nR,nG,nB;
		int nField = _stscanf_s(szBuf,_T("RGB(%d,%d,%d)"),&nR,&nG,&nB);
		if(nField == 3)
		{
			crValue = RGB(nR,nG,nB);
			return ERROR_SUCCESS;
		}
		else
			lRtn = -1;
	}
	
	crValue = crDefaultValue;
	
	return lRtn;
}

LONG CWzRegKey::GetRect(LPCTSTR lpszValueName, RECT& rcValue)
{
	WZ_ASSERT(m_hKey != NULL);

	TCHAR szBuf[128];
	LONG lRtn = GetString(lpszValueName,szBuf,128,NULL);
	if(lRtn == ERROR_SUCCESS)
	{
		int nField = _stscanf_s(szBuf,_T("RECT(%d,%d,%d,%d)"),&rcValue.left,&rcValue.top,&rcValue.right,&rcValue.bottom);
		if(nField != 4)
			lRtn = -1;
	}
	
	return lRtn;
}

LONG CWzRegKey::GetBool(LPCTSTR lpszValueName, BOOL& bValue, BOOL bDefault)
{
	WZ_ASSERT(m_hKey != NULL);

	TCHAR szBuf[128];

	bValue = bDefault;
	LONG lRtn = GetString(lpszValueName,szBuf,128,NULL);
	if(lRtn == ERROR_SUCCESS)
	{
		if(lstrcmp(szBuf,_T("TRUE")) == 0)
			bValue = TRUE;
		else
			bValue = FALSE;
	}
	
	return lRtn;
}

LONG CWzRegKey::GetBinary(LPCTSTR lpszValueName, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	WZ_ASSERT(m_hKey != NULL);
	
	return ::RegQueryValueEx(m_hKey,lpszValueName,NULL,lpType,lpData,lpcbData);
}

LONG CWzRegKey::SetInteger(LPCTSTR lpszValueName, int nValue)
{
	WZ_ASSERT(m_hKey != NULL);
	DWORD dwValue = nValue;
	LONG lRtn = ::RegSetValueEx(m_hKey,lpszValueName,0,REG_DWORD,(CONST BYTE *)&dwValue,sizeof(DWORD));
	
	return lRtn;
}

LONG CWzRegKey::SetDword(LPCTSTR lpszValueName, DWORD dwValue)
{
	WZ_ASSERT(m_hKey != NULL);
	LONG lRtn = ::RegSetValueEx(m_hKey,lpszValueName,0,REG_DWORD,(CONST BYTE *)&dwValue,sizeof(DWORD));
	
	return lRtn;
}

LONG CWzRegKey::SetString(LPCTSTR lpszValueName, LPCTSTR lpValue)
{
	WZ_ASSERT(m_hKey != NULL);
	DWORD dwSize = (lstrlen(lpValue) + 1) * sizeof(TCHAR);// include null char
	LONG lRtn = ::RegSetValueEx(m_hKey,lpszValueName,0,REG_SZ,(CONST BYTE *)lpValue,dwSize);
	
	return lRtn;
}

LONG CWzRegKey::SetPoint(LPCTSTR lpszValueName, POINT ptValue)
{
	WZ_ASSERT(m_hKey != NULL);
	TCHAR szBuf[128];
	wsprintf(szBuf,_T("POINT(%d,%d)"),ptValue.x,ptValue.y);
	LONG lRtn = SetString(lpszValueName,szBuf);
	
	return lRtn;
}

LONG CWzRegKey::SetColor(LPCTSTR lpszValueName, COLORREF crValue)
{
	WZ_ASSERT(m_hKey != NULL);
	TCHAR szBuf[128];
	wsprintf(szBuf,_T("RGB(%d,%d,%d)"),GetRValue(crValue), GetGValue(crValue), GetBValue(crValue));
	LONG lRtn = SetString(lpszValueName,szBuf);
	
	return lRtn;
}

LONG CWzRegKey::SetRect(LPCTSTR lpszValueName, RECT rcValue)
{
	WZ_ASSERT(m_hKey != NULL);
	TCHAR szBuf[128];
	wsprintf(szBuf,_T("RECT(%d,%d,%d,%d)"),rcValue.left,rcValue.top,rcValue.right,rcValue.bottom);
	LONG lRtn = SetString(lpszValueName,szBuf);
	
	return lRtn;
}

LONG CWzRegKey::SetBool(LPCTSTR lpszValueName, BOOL bValue)
{
	WZ_ASSERT(m_hKey != NULL);
	TCHAR szBuf[128];
	lstrcpy(szBuf,(bValue == TRUE) ? _T("TRUE") : _T("FALSE"));
	LONG lRtn = SetString(lpszValueName,szBuf);
	
	return lRtn;
}

LONG CWzRegKey::SetBinary(LPCTSTR lpszValueName, DWORD dwType, CONST BYTE *lpData, DWORD cbData)
{
	WZ_ASSERT(m_hKey != NULL);

	return ::RegSetValueEx(m_hKey,lpszValueName,0,dwType,lpData,cbData);
}

LONG CWzRegKey::EnumValue(DWORD dwIndex,LPTSTR lpValueName,LPDWORD lpcValueName,LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData)
{
	WZ_ASSERT(m_hKey != NULL);

	return ::RegEnumValue(m_hKey,dwIndex,lpValueName,lpcValueName,NULL,lpType,lpData,lpcbData);
}

LONG CWzRegKey::EnumSubKey(DWORD dwIndex,LPTSTR lpName,LPDWORD lpcName,LPTSTR lpClass,LPDWORD lpcClass, PFILETIME lpftLastWriteTime)
{
	WZ_ASSERT(m_hKey != NULL);

	return ::RegEnumKeyEx(m_hKey,dwIndex,lpName,lpcName,NULL,lpClass,lpcClass,lpftLastWriteTime);
}

LONG CWzRegKey::DeleteValue(LPCTSTR lpszValueName)
{
	WZ_ASSERT(m_hKey != NULL);

	return ::RegDeleteValue(m_hKey,lpszValueName);
}

LONG CWzRegKey::DeleteSubKey(LPCTSTR lpszSubKey, BOOL bRecurse)
{
	WZ_ASSERT(m_hKey != NULL);
	LONG lRtn = ERROR_SUCCESS;
	if(bRecurse)
	{
		CWzRegKey key;
		LONG lRes = key.OpenKey(m_hKey,lpszSubKey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		FILETIME time;
		TCHAR szBuffer[256];
		DWORD dwSize = 256;
		while(key.EnumSubKey(0,szBuffer,&dwSize,NULL,NULL,&time) == ERROR_SUCCESS)
		{
			lRes = key.DeleteSubKey(szBuffer,bRecurse);//bRecurse = TRUE
			if (lRes != ERROR_SUCCESS)
				return lRes;
			dwSize = 256;
		}

		key.CloseKey();
	}

	//最后删除这个key
	lRtn = ::RegDeleteKey(m_hKey,lpszSubKey);

	return lRtn;
}
