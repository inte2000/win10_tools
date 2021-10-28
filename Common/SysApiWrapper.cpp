#include "stdafx.h"
#include <shlobj.h>
#include <shellapi.h>
#include <dwmapi.h>
#include <strsafe.h>
#include "SysApiWrapper.h"


BOOL VerifyWindowsVersion(DWORD major, DWORD minor, DWORD servicePack, DWORD buildnumber, int op)
{
    OSVERSIONINFOEX ove = { 0 };
    DWORDLONG dwlConditionMask = 0;

    ove.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    ove.dwMajorVersion = major;
    ove.dwMinorVersion = minor;
    ove.wServicePackMajor = (WORD)servicePack;
    ove.dwBuildNumber = buildnumber;

    DWORD mask = VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR | VER_BUILDNUMBER; //VER_SERVICEPACKMINOR
    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, op);
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, op);
    VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMAJOR, op);
    VER_SET_CONDITION(dwlConditionMask, VER_BUILDNUMBER, op);

    return VerifyVersionInfo(&ove, mask, dwlConditionMask);
}

TString GetAppPathName(HMODULE hMod, LPCTSTR lpName)
{
	TCHAR szBuf[MAX_PATH];

	DWORD len = ::GetModuleFileName(hMod, szBuf, MAX_PATH);
    szBuf[len] = 0;

    TCHAR *pSlash = _tcsrchr(szBuf, _T('\\'));
    if (pSlash != nullptr)
    {
        std::size_t space = MAX_PATH - _tcslen(szBuf) + _tcslen(pSlash + 1);
        _tcscpy_s(pSlash + 1, space, lpName);
    }
    else
    {
        _tcscpy_s(szBuf, MAX_PATH, lpName);
    }

    return TString(szBuf);
}

BOOL IsDirectoryExists(const TString& path)
{
    if (path.empty())
        return FALSE;

    if (path.find(_T("::"), 0) == 0)
        return FALSE;

    return ::PathFileExists(path.c_str());
}

BOOL IsFileExists(const TString& path) 
{
    if (path.empty())
        return FALSE;

    if (path.find(_T("::"), 0) == 0)
        return FALSE;

    return ::PathFileExists(path.c_str());
}

HIMAGELIST GetSystemImageList(BOOL bLargeIcon)
{
    HIMAGELIST hSysImgList = NULL;
    SHFILEINFO shfi;
    ::ZeroMemory(&shfi, sizeof(SHFILEINFO));
    UINT uFlags = SHGFI_SYSICONINDEX | SHGFI_ICON | (bLargeIcon ? SHGFI_LARGEICON : SHGFI_SMALLICON);
    hSysImgList = (HIMAGELIST)::SHGetFileInfo((LPCTSTR)_T(""), 0, &shfi, sizeof(SHFILEINFO), uFlags);
    if (hSysImgList == NULL)
    {
        return NULL;
    }

    return hSysImgList;
}

TString TStringFromWStr(LPWSTR wstr)
{
    TString strRtn;
#ifdef UNICODE
    strRtn = wstr;
#else
    //don't forget ...
#endif

    return std::move(strRtn);
}

WCHAR* WStrFromTString(WCHAR* wstrBuf, int bufChs, const TString& tstr)
{
#ifdef UNICODE
    StringCchCopyW(wstrBuf, bufChs, tstr.c_str());
#else
    int _convert = ::MultiByteToWideChar(CP_ACP, 0, (LPCCH)tstr.c_str(), -1, wstrBuf, bufChs);
    wstrBuf[_convert - 1] = 0;
#endif
    return wstrBuf;
}
//_T("All\0*.*\0")
TString WzGetOpenFileName(HWND hWnd, LPCTSTR lpszInitDir, LPCTSTR lpszFilter)
{
    OPENFILENAME ofn;       // common dialog box structure
    TCHAR szFile[MAX_PATH] = { 0 };       // if using TCHAR macros

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = lpszFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = lpszInitDir;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING | OFN_EXPLORER;

    SStringT this_path;
    if (::GetOpenFileName(&ofn) == TRUE)
    {
        return TString(ofn.lpstrFile);
    }

    return TString(_T(""));
}

BOOL WzSetDesktopWallpaper(LPCTSTR lpszFileName)
{
    return ::SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)lpszFileName, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
}

SStringT WzGetDesktopWallpaper()
{
    SStringT strWallpaper;
    ::SystemParametersInfo(SPI_GETDESKWALLPAPER, MAX_PATH, (PVOID)strWallpaper.GetBuffer(MAX_PATH), 0);
    strWallpaper.ReleaseBuffer();

    return strWallpaper;
}

SStringT GetAppLocalFolder()
{
    SStringT strFolder;
    ::SHGetSpecialFolderPath(NULL, strFolder.GetBuffer(MAX_PATH), CSIDL_LOCAL_APPDATA, FALSE);
    strFolder.ReleaseBuffer();

    return strFolder;
}

SStringT GetWindowsFolder()
{
    SStringT strFolder;
    ::SHGetSpecialFolderPath(NULL, strFolder.GetBuffer(MAX_PATH), CSIDL_WINDOWS, FALSE);
    strFolder.ReleaseBuffer();

    return strFolder;
}



