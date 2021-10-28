// taskbar.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "taskbar.h"

extern HINSTANCE g_hDllInstance;

#pragma data_seg("Shared")  
int g_nTaskbarMode = 8;
int g_nTransOrOpac = 0;
DWORD abgrColor = 0xFF000000;
BOOL g_MaximunWndMatch = FALSE;
#pragma data_seg()  

#pragma comment(linker, "/section:Shared,rws")

HHOOK hCbtProc = NULL;

LRESULT CALLBACK TaskbarCBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HCBT_ACTIVATE)
	{
	}

	return ::CallNextHookEx(hCbtProc, nCode, wParam, lParam);
}

TASKBAR_API BOOL WINAPI InstallHook(void)
{
	//hDMSShellProc = SetWindowsHookEx(WH_SHELL, (HOOKPROC)(DMSShellProc), g_hDllInstance, 0);
	hCbtProc = ::SetWindowsHookEx(WH_CBT, (HOOKPROC)TaskbarCBTProc, g_hDllInstance, 0);
	if (hCbtProc != NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

TASKBAR_API void WINAPI UninstallHook(void)
{
	if (hCbtProc != NULL)
	{
		::UnhookWindowsHookEx(hCbtProc);
		hCbtProc = NULL;
	}
}

TASKBAR_API void WINAPI SetTaskbarMode(int mode, int transopca)
{
	g_nTaskbarMode = mode;
	g_nTransOrOpac = transopca;
}

TASKBAR_API void WINAPI SetOptions(BOOL bMaximumWndMatch)
{
	g_MaximunWndMatch = bMaximumWndMatch;
}