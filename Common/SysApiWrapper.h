#pragma once

#include "std_inc.h"

BOOL VerifyWindowsVersion(DWORD major, DWORD minor, DWORD servicePack, DWORD buildnumber, int op);


TString GetAppPathName(HMODULE hMod, LPCTSTR lpName);

BOOL IsDirectoryExists(const TString& path);
BOOL IsFileExists(const TString& path);

HIMAGELIST GetSystemImageList(BOOL bLargeIcon);
TString TStringFromWStr(LPWSTR wstr);
WCHAR* WStrFromTString(WCHAR* wstrBuf, int bufChs, const TString& tstr);

TString WzGetOpenFileName(HWND hWnd, LPCTSTR lpszInitDir, LPCTSTR lpszFilter);
BOOL WzSetDesktopWallpaper(LPCTSTR lpszFileName);
SStringT WzGetDesktopWallpaper();
SStringT GetAppLocalFolder();
SStringT GetWindowsFolder();

