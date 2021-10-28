#include "stdafx.h"
#include "StringFunc.h"

TString TStringFromChars(const char *ansiChars)
{
    TString s;
	if((ansiChars != NULL) && (strlen(ansiChars) > 0) )
	{
#ifdef UNICODE
		int strLen = (int)strlen(ansiChars) + 1;
        LPTSTR p = new(std::nothrow) TCHAR[strLen * 2];
        if (p != nullptr)
        {
		    ::MultiByteToWideChar(CP_ACP,0,ansiChars,-1,p,strLen);
            s = p;
            delete[] p;
        }
#else
		s = ansiChars;
#endif
	}
	return std::move(s);
}

TString TStringFromWChars(const WCHAR *wChars)
{
    TString s;
	if((wChars != NULL) && (wcslen(wChars) > 0))
	{
#ifdef UNICODE
		s = wChars;
#else
    int nCount = ::WideCharToMultiByte(CP_ACP, 0, wChars, -1, NULL, 0, NULL, NULL);
    LPTSTR p = new(std::nothrow) TCHAR[nCount + 2];
    if(p)
    {
	    ::WideCharToMultiByte(CP_ACP, 0 , wChars, -1, p, nCount, NULL, NULL);
        s = p;
        delete[] p;
    }
#endif
	}
	return std::move(s);
}

int AnsiCharsFromTString(char *buf, UINT bufSize, const TString& cstr)
{
    int nCount = 0;
	if(buf)
	{
		buf[0] = 0;
#ifdef UNICODE
        nCount = ::WideCharToMultiByte(CP_ACP, 0, cstr.c_str(), cstr.length(), buf, bufSize, NULL, NULL);
		buf[nCount] = 0;
#else
        nCount = cstr.length();
		strncpy(buf, cstr.c_str(), bufSize);
		buf[bufSize - 1] = 0;
#endif
	}
	
	return nCount;
}

int WCharsFromTString(WCHAR* buf, UINT bufSize, const TString& cstr)
{
    int nCount = 0;
    if (buf)
    {
#ifdef UNICODE
        nCount = cstr.length();
        wcsncpy(buf, cstr.c_str(), bufSize);
        buf[bufSize - 1] = 0;
#else
        nCount = ::MultiByteToWideChar(CP_ACP, 0, cstr.c_str(), -1, buf, bufSize);
        buf[bufSize - 1] = 0;
#endif
    }

    return nCount;
}

std::string AnsiStringFromTString(const TString& cstr)
{
    std::string rtn;
    int lenchars = cstr.length() * 2 + 2;
    char* buf = new(std::nothrow) char[lenchars];
    if (buf != nullptr)
    {
        int len = AnsiCharsFromTString(buf, lenchars, cstr);
        if (len > 0)
            rtn = buf;

        delete[] buf;
    }

    return std::move(rtn);
}

TString TStringFromAnsiString(const std::string& ansiString)
{
    return TStringFromChars(ansiString.c_str());
}

SStringT GetFileBaseName(LPCTSTR lpszPathName)
{
    int length = lstrlen(lpszPathName);
    LPCTSTR lpStart = lpszPathName;
    LPCTSTR lpEnd = lpStart + length - 1;
    LPTSTR lpMatchSlash = StrRChr(lpStart, lpEnd, _T('\\'));
    if (lpMatchSlash != nullptr)
        lpStart = lpMatchSlash + 1;

    SStringT strname = lpStart;
    LPTSTR lpMatchDot = StrRChr(lpStart, lpEnd, _T('.'));
    if (lpMatchDot != nullptr)
    {
        int baselen = lpMatchDot - lpStart;
        strname = strname.Mid(0, baselen);
    }

    return strname;
}

