// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "SubclassMgmt.h"
#include "ThemeMode.h"
#include "HookShellTray.h"
#include <vector>
#include <string>

HINSTANCE g_hDllInstance = NULL;       // DLLµÄÊµÀý¾ä±ú
UINT MSG_SETTASKBAR_APPEARANCE = 0;
std::vector<TASKBAR_HOOK_INFO> g_taskbars;


void HookAllTaskbars()
{
    HWND hTaskbar = ::FindWindow(_T("Shell_TrayWnd"), NULL);
    if ((hTaskbar != NULL) && !IsTaskbarHooked(hTaskbar))
    {
        TASKBAR_HOOK_INFO hook = { hTaskbar, ::MonitorFromWindow(hTaskbar, MONITOR_DEFAULTTOPRIMARY), HookShellTrayWindow(hTaskbar) };
        g_taskbars.push_back(hook);
    }
    HWND h2ndTaskbar = ::FindWindowEx(0, NULL, _T("Shell_SecondaryTrayWnd"), NULL);
    while (h2ndTaskbar != NULL)
    {
        if (!IsTaskbarHooked(h2ndTaskbar))
        {
            TASKBAR_HOOK_INFO secondryHook = { h2ndTaskbar, ::MonitorFromWindow(h2ndTaskbar, MONITOR_DEFAULTTOPRIMARY), HookShellTrayWindow(h2ndTaskbar) };
            g_taskbars.push_back(secondryHook);
        }
    }
}

void UnhookAllTaskbars()
{
    for (auto& x : g_taskbars)
    {
        UnhookShellTrayWindow(x.hTaskbar);
    }
    g_taskbars.clear();
}

BOOL GetCurrentProcessName(LPTSTR buf, UINT bufChs)
{
    TCHAR szProcPath[MAX_PATH];
    ::GetModuleFileName(NULL, szProcPath, MAX_PATH);
    TCHAR* pNameIndex = wcsrchr(szProcPath, L'\\');
    if (pNameIndex == NULL) 
        return FALSE;
    pNameIndex++;
    wcscpy_s(buf, bufChs, pNameIndex);
    return TRUE;
}

BOOL OnProcessAttach(HMODULE hModule)
{
    g_hDllInstance = (HINSTANCE)hModule;

#ifdef _DEBUG
    TCHAR dbgInfo[256];
    wsprintf(dbgInfo, _T("!!!!!Hook explorer process: %u \n"), ::GetCurrentProcessId());
    OutputDebugString(dbgInfo);
#endif
    ::DisableThreadLibraryCalls(hModule);
    InitSubclassMgmt();
    MSG_SETTASKBAR_APPEARANCE = ::RegisterWindowMessage(_T("UIMON_SETTASKBAR"));
    TCHAR szProcName[128] = { 0 };
    if (!GetCurrentProcessName(szProcName, 128))
        return FALSE;
    if ( _wcsicmp(szProcName, L"ui_mon.exe") == 0) //our main process
        return TRUE;
    if (_wcsicmp(szProcName, L"explorer.exe") != 0) //only care about explorer.exe
        return FALSE;

    if (!InitThemeMode())
        return FALSE;

    HookAllTaskbars();

#ifdef _DEBUG
    wsprintf(dbgInfo, _T("!!!!!Hook explorer OnProcessAttach OK: %u, taskbars = %d \n"), ::GetCurrentProcessId(), g_taskbars.size());
    OutputDebugString(dbgInfo);
#endif

    return TRUE;
}

void OnProcessDetach(HMODULE hModule)
{
#ifdef _DEBUG
    TCHAR dbgInfo[256];
    wsprintf(dbgInfo, _T("!!!!!Unhook explorer process: %u, taskbars = %d \n"), ::GetCurrentProcessId(), g_taskbars.size());
    OutputDebugString(dbgInfo);
#endif

    UnhookAllTaskbars();
    ReleaseSubclassMgmt();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    BOOL bRtn = TRUE;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        bRtn = OnProcessAttach(hModule);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        OnProcessDetach(hModule);
        break;
    }
    return bRtn;
}

