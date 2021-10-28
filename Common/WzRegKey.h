#ifndef __WZREGKEY_H__
#define __WZREGKEY_H__

class CWzRegKey
{
public:
	CWzRegKey();
	CWzRegKey(HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired = KEY_ALL_ACCESS);
	~CWzRegKey();

public:
	LONG CreateKey(HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired = KEY_ALL_ACCESS);
	LONG OpenKey(HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired = KEY_ALL_ACCESS);
	LONG CloseKey();
	BOOL IsValid() const { return (m_hKey != NULL); }

	LONG GetInteger(LPCTSTR lpszValueName, int& nValue, int nDefaultValue = 0);
	LONG GetDword(LPCTSTR lpszValueName, DWORD& dwValue, DWORD dwDefaultValue = 0);
	LONG GetString(LPCTSTR lpszValueName, LPTSTR lpValueBuf, int nBufSize, LPCTSTR lpDefaultValue = NULL);
	LONG GetPoint(LPCTSTR lpszValueName, POINT& ptValue);
	LONG GetColor(LPCTSTR lpszValueName, COLORREF& crValue, COLORREF crDefaultValue = RGB(0,0,0));
	LONG GetRect(LPCTSTR lpszValueName, RECT& rcValue);
	LONG GetBool(LPCTSTR lpszValueName, BOOL& bValue, BOOL bDefault = FALSE);
	LONG GetBinary(LPCTSTR lpszValueName, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);

	LONG SetInteger(LPCTSTR lpszValueName, int nValue);
	LONG SetDword(LPCTSTR lpszValueName, DWORD dwValue);
	LONG SetString(LPCTSTR lpszValueName, LPCTSTR lpValue);
	LONG SetPoint(LPCTSTR lpszValueName, POINT ptValue);
	LONG SetColor(LPCTSTR lpszValueName, COLORREF crValue);
	LONG SetRect(LPCTSTR lpszValueName, RECT rcValue);
	LONG SetBool(LPCTSTR lpszValueName, BOOL bValue);
	LONG SetBinary(LPCTSTR lpszValueName, DWORD dwType, CONST BYTE *lpData, DWORD cbData);

	LONG EnumValue(DWORD dwIndex,LPTSTR lpValueName,LPDWORD lpcValueName,LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData);
	LONG EnumSubKey(DWORD dwIndex,LPTSTR lpName,LPDWORD lpcName,LPTSTR lpClass,LPDWORD lpcClass, PFILETIME lpftLastWriteTime);

	LONG DeleteValue(LPCTSTR lpszValueName);
	LONG DeleteSubKey(LPCTSTR lpszSubKey, BOOL bRecurse = FALSE);
	
protected:
	HKEY         m_hKey;
};

#endif