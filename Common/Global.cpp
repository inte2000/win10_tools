#include "stdafx.h"
#include "Global.h"
#include "ThemeMode.h"
#include "SysApiWrapper.h"
#include "NtdllFunc.h"
#include "UxThemeFunc.h"
#include "User32Func.h"
#include "TaskbarFunc.h"
#include "WzRegKey.h"

CTaskbarFunc g_taskbarDll;


BOOL InitGlobalData()
{
    if (!InitThemeMode())
        return FALSE;

    if (!g_taskbarDll.LoadFunctions())
        return FALSE;

    if (!IsSystemMatchRequest())
        return FALSE;

    DWORD crColorization;
    BOOL OpaqueBlend;
    DwmGetColorizationColor(&crColorization, &OpaqueBlend);

    return TRUE;
}

BOOL IsSystemMatchRequest()
{
    return VerifyWindowsVersion(10, 0, 0, 17763, VER_GREATER_EQUAL);
}

BOOL SwitchSystemToLightMode()
{
    ::Sleep(1000);

    if (!SwitchPersonalizeTheme(FALSE))
        return FALSE;

    return TRUE;
}

BOOL SwitchSystemToDarkMode()
{
    if (!SwitchPersonalizeTheme(TRUE))
        return FALSE;
    
    ::Sleep(1000);
    return TRUE;
}

BOOL IsClassWindow(HWND hWnd, LPCTSTR matchclassName)
{
    TCHAR className[128] = { 0 };
    ::GetClassName(hWnd, className, 128);
    if (StrCmpI(matchclassName, className) == 0)
        return TRUE;

    return FALSE;
}

BOOL IsSystemSettingProcess(LPCTSTR processName)
{
    //if (StrStr(processName, _T("SystemSettings.exe")) != NULL)
    if (StrStr(processName, _T("ApplicationFrameHost.exe")) != NULL)
        return TRUE;

    return FALSE;
}

typedef struct tagEnumCloseWnd
{
    HWND hFindWnd;
    DWORD dwProcessId;
}EnumCloseWnd;

BOOL CALLBACK FindAndCloseSettingWnd(HWND hwnd, LPARAM lParam)
{
    BOOL bFind = FALSE;
    EnumCloseWnd* pPara = (EnumCloseWnd *)lParam;
    if (IsClassWindow(hwnd, _T("ApplicationFrameWindow")))
    {
        TCHAR szProcessName[MAX_PATH] = { 0 };
        DWORD dwProcId = 0;
        ::GetWindowThreadProcessId(hwnd, &dwProcId);
        HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcId);
        if (hProcess != NULL)
        {
            DWORD dwRtn = ::GetProcessImageFileName(hProcess, szProcessName, MAX_PATH);
            if ((dwRtn > 0) && IsSystemSettingProcess(szProcessName))
            {
                pPara->hFindWnd = hwnd;
                pPara->dwProcessId = dwProcId;
                ::ShowWindow(hwnd, SW_HIDE);
                ::Sleep(1000);
                ::WaitForInputIdle(hProcess, INFINITE);
                ::SendMessage(hwnd, WM_CLOSE, NULL, NULL);
                STRACE(_T("Find window %x, closed!\n"), hwnd);
                bFind = TRUE;
            }
            ::CloseHandle(hProcess);
        }
    }

    return !bFind;
}

//C:\Windows\ImmersiveControlPanel\SystemSettings.exe
BOOL StartProcessAndWait(HWND hWnd, LPCTSTR exeFile, LPCTSTR param, int waittime)
{
    SHELLEXECUTEINFO sei = { 0 };
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS | SEE_MASK_UNICODE;
    sei.hwnd = hWnd;
    sei.lpVerb = _T("open");
    sei.lpFile = exeFile;
    sei.lpParameters = param;
    sei.nShow = SW_HIDE| SW_SHOWNOACTIVATE;// SW_SHOW;

    BOOL rtn = ::ShellExecuteEx(&sei);
    if (rtn && (int)sei.hInstApp > 32)
    {
        TCHAR szProcessName[MAX_PATH] = { 0 };
        DWORD dwRtn = ::GetProcessImageFileName(sei.hProcess, szProcessName, MAX_PATH);
        int thisWait = 0;
        EnumCloseWnd enumResult = { NULL, 0 };
        do
        {
            ::EnumWindows(FindAndCloseSettingWnd, (LPARAM)&enumResult);
            if (enumResult.hFindWnd != NULL)
            {
                break;
            }
            ::Sleep(100);
            thisWait += 100;
        } while (thisWait <= waittime);
        //::WaitForSingleObject(sei.hProcess, INFINITE);
        ::CloseHandle(sei.hProcess);
    }

    return rtn;
}

//rundll32.exe %SystemRoot%\system32\shell32.dll,Control_RunDLL %SystemRoot%\system32\desk.cpl desk,@Themes /Action:OpenTheme /file:"C:\Windows\Resources\Themes\aero.theme"
BOOL SwitchSystemTheme(HWND hWnd, LPCTSTR strThemeFile)
{
    //TCHAR szParam[512] = { 0 };
    //wsprintf(szParam, _T(" %%SystemRoot%%\\system32\\shell32.dll,Control_RunDLL %%SystemRoot%%\\system32\\desk.cpl desk,@Themes /Action:OpenTheme /file:\"%s\""), strThemeFile);
    BOOL rtn = StartProcessAndWait(hWnd, strThemeFile, NULL, 5000);

    return rtn;
}

LPCTSTR lpszRunKeyname = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
BOOL AddRegistryAutorunEntry(LPCTSTR entryname, LPCTSTR exepath, LPCTSTR param)
{
    CWzRegKey regKey(HKEY_CURRENT_USER, lpszRunKeyname, KEY_WRITE | KEY_READ);
    if (!regKey.IsValid())
        return FALSE;

    TString cmdLine = exepath;
    if (param != nullptr)
    {
        cmdLine += _T(" ");
        cmdLine += param;
    }

    if (regKey.SetString(entryname, cmdLine.c_str()) != ERROR_SUCCESS)
        return FALSE;

    return TRUE;
}

BOOL RemoveRegistryAutorunEntry(LPCTSTR entryname)
{
    CWzRegKey regKey(HKEY_CURRENT_USER, lpszRunKeyname, KEY_WRITE | KEY_READ);
    if (!regKey.IsValid())
        return FALSE;

    if (regKey.DeleteValue(entryname) != ERROR_SUCCESS)
        return FALSE;

    return TRUE;
}

BOOL InstallTaskbarHook()
{
    return g_taskbarDll.InstallHook();
    //return TRUE;
}

void InitSharedTaskbarMode(int nTaskbarMode, int nTransopca)
{
    g_taskbarDll.SetTaskbarMode(nTaskbarMode, nTransopca);
}

void SetharedTaskbarOptions(BOOL bMaximumWndMatch)
{
    g_taskbarDll.SetOptions(bMaximumWndMatch);
}

void UninstallTaskbarHook()
{
    g_taskbarDll.UninstallHook();
}

