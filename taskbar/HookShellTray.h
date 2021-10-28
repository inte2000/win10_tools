#pragma once

#include <Windows.h>

typedef struct tagTASKBAR_HOOK_INFO
{
	HWND hTaskbar;
	HMONITOR hmon;
	CHookShellTray* pHooker;
	//TASKBARSTATE state;
}TASKBAR_HOOK_INFO;


class CHookShellTray final
{
	//friend LRESULT CALLBACK CHookShellTray::HookShellTrayWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend CHookShellTray* HookShellTrayWindow(HWND hTrayWnd);
	friend void UnhookShellTrayWindow(HWND hTrayWnd);

public:
	CHookShellTray();
	~CHookShellTray();

protected:
	BOOL Subclass(HWND hWnd);
	void Unsubclass();
	LRESULT OnSetTaskbar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnActive(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnEraseBkGnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcActive(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CallDefWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK HookShellTrayWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWnd;
	WNDPROC m_oriWndProc;
};

CHookShellTray* HookShellTrayWindow(HWND hTrayWnd);
void UnhookShellTrayWindow(HWND hTrayWnd);
BOOL IsTaskbarHooked(HWND hTaskbar);
