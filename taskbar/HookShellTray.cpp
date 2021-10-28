#include "framework.h"
#include "SubclassMgmt.h"
#include "HookShellTray.h"
#include "ThemeMode.h"

const UINT gActiveTimerID = 121;
extern UINT MSG_SETTASKBAR_APPEARANCE;
extern std::vector<TASKBAR_HOOK_INFO> g_taskbars;

extern int g_nTaskbarMode;
extern int g_nTransOrOpac;
extern BOOL g_MaximunWndMatch;

void SetTaskbarBluring(int nTaskbarMode, int nTaskbarTransparency, int nAccentOpacity)
{
	DWORD abgrColor = 0x383838;
	if (!IsDarkModeEnabled())
		abgrColor = 0x999999;

	if (nTaskbarMode == 0)
	{
		abgrColor = 0;
		for (const auto& x : g_taskbars)
			SetWindowBlurring(x.hTaskbar, nTaskbarMode, abgrColor);
	}
	else if (nTaskbarMode == 2)
	{
		//abgrColor = 0x0000FFFF;
		DWORD Transparency = nTaskbarTransparency;
		Transparency <<= 24;
		abgrColor |= Transparency;
		for (const auto& x : g_taskbars)
			SetWindowBlurring(x.hTaskbar, nTaskbarMode, abgrColor);
	}
	else
	{
		DWORD Opacity = nAccentOpacity;
		Opacity <<= 24;
		//abgrColor &= 0x00FFFF00;
		abgrColor |= Opacity;
		//rgbaColor = 0x66000000;
		for (const auto& x : g_taskbars)
			SetWindowBlurring(x.hTaskbar, nTaskbarMode, abgrColor);
	}
}

CHookShellTray::CHookShellTray()
{
	m_hWnd = NULL;
	m_oriWndProc = NULL;
}

CHookShellTray::~CHookShellTray()
{
}

LRESULT CHookShellTray::OnSetTaskbar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nTaskbarMode = int(wParam);
	DWORD abgrColor = (DWORD)lParam;
	int nTransOpac = (abgrColor & 0xFF000000) >> 24;

	if (nTaskbarMode == 8)  //for hook state test
		return 42;

	g_nTaskbarMode = nTaskbarMode;
	g_nTransOrOpac = nTransOpac;

#ifdef _DEBUG
	TCHAR dbgInfo[256];
	wsprintf(dbgInfo, _T("!!!!!CHookShellTray::OnSetTaskbar(%d) process: %u\n"), nTaskbarMode, ::GetCurrentProcessId());
	OutputDebugString(dbgInfo);
#endif

	SetTaskbarBluring(nTaskbarMode, nTransOpac, nTransOpac);
	return 0;
}

LRESULT CHookShellTray::OnTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	TCHAR dbgInfo[256];
	wsprintf(dbgInfo, _T("!!!!!CHookShellTray(0x%x) OnTimer : %u \n"), hWnd, ::GetCurrentProcessId());
	OutputDebugString(dbgInfo);
#endif

	if ((UINT)wParam == gActiveTimerID)
	{
		::KillTimer(hWnd, gActiveTimerID);
		SetTaskbarBluring(g_nTaskbarMode, g_nTransOrOpac, g_nTransOrOpac);
	}

	return CallDefWndProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CHookShellTray::OnEraseBkGnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lr = CallDefWndProc(hWnd, uMsg, wParam, lParam);

#ifdef _DEBUG
	TCHAR dbgInfo[256];
	wsprintf(dbgInfo, _T("!!!!!CHookShellTray(0x%x) OnEraseBkGnd() : %u \n"), hWnd, ::GetCurrentProcessId());
	OutputDebugString(dbgInfo);
#endif
	::SetTimer(m_hWnd, gActiveTimerID, 10, NULL);

	return lr;
}

LRESULT CHookShellTray::OnActive(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int code = LOWORD(wParam);
	HWND hPeerWnd = HWND(lParam);

	LRESULT lr = CallDefWndProc(hWnd, uMsg, wParam, lParam);

#ifdef _DEBUG
	TCHAR dbgInfo[256];
	wsprintf(dbgInfo, _T("!!!!!CHookShellTray(0x%x) OnActive(%d) : %u \n"), hWnd, code, ::GetCurrentProcessId());
	OutputDebugString(dbgInfo);
#endif

	//if (code == WA_INACTIVE)
	{
		::SetTimer(m_hWnd, gActiveTimerID, 10, NULL);
	}

	return lr;
}

LRESULT CHookShellTray::OnNcActive(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bActive = BOOL(wParam);

	LRESULT lr = CallDefWndProc(hWnd, uMsg, wParam, lParam);
#ifdef _DEBUG
	TCHAR dbgInfo[256];
	wsprintf(dbgInfo, _T("!!!!!CHookShellTray(0x%x) OnNcActive(%d) : %u \n"), hWnd, bActive, ::GetCurrentProcessId());
	OutputDebugString(dbgInfo);
#endif

	return lr;
}

LRESULT CHookShellTray::OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lr = CallDefWndProc(hWnd, uMsg, wParam, lParam);
	Unsubclass();
	RemoveSubclassItem(hWnd);
	delete this;

#ifdef _DEBUG
	TCHAR dbgInfo[256];
	wsprintf(dbgInfo, _T("!!!!!CHookShellTray(0x%x) OnDestroy() : %u \n"), hWnd, ::GetCurrentProcessId());
	OutputDebugString(dbgInfo);
#endif

	return lr;
}

LRESULT CHookShellTray::CallDefWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_oriWndProc(hWnd, uMsg, wParam, lParam);
}

BOOL CHookShellTray::Subclass(HWND hWnd)
{
	if ((hWnd != NULL) && (m_oriWndProc == NULL))
	{
		m_oriWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)HookShellTrayWndProc);
		if (m_oriWndProc != NULL)
		{
			m_hWnd = hWnd;
			return TRUE;
		}
		UINT err = ::GetLastError();
	}

	return FALSE;
}


void CHookShellTray::Unsubclass()
{
	if ((m_hWnd != NULL) && (m_oriWndProc != NULL))
	{
		::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_oriWndProc);
		m_hWnd = NULL;
		m_oriWndProc = NULL;
	}
}

LRESULT CALLBACK CHookShellTray::HookShellTrayWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CHookShellTray*pThisWnd = FindSubclassWnd(hWnd);
	if (pThisWnd == nullptr)
	{
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	if (uMsg == MSG_SETTASKBAR_APPEARANCE)
	{
		return pThisWnd->OnSetTaskbar(hWnd, uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
	case WM_TIMER:
		return pThisWnd->OnTimer(hWnd, uMsg, wParam, lParam);
	case WM_ERASEBKGND:
		return pThisWnd->OnEraseBkGnd(hWnd, uMsg, wParam, lParam);
//	case WM_PRINTCLIENT:
	case WM_ACTIVATE:
		return pThisWnd->OnActive(hWnd, uMsg, wParam, lParam);
	case WM_NCACTIVATE:
		return pThisWnd->OnNcActive(hWnd, uMsg, wParam, lParam);
	case WM_DESTROY:
		return pThisWnd->OnDestroy(hWnd, uMsg, wParam, lParam);
	default:
		break;
	}
	return pThisWnd->CallDefWndProc(hWnd, uMsg, wParam, lParam);
}

CHookShellTray* HookShellTrayWindow(HWND hTrayWnd)
{
	CHookShellTray* pHooker = new CHookShellTray();
	if (pHooker == nullptr)
		return nullptr;

	if (pHooker->Subclass(hTrayWnd))
	{
		AddSubclassItem(hTrayWnd, pHooker);
		return pHooker;
	}

	delete pHooker;
	return nullptr;
}

void UnhookShellTrayWindow(HWND hTrayWnd)
{
	CHookShellTray* pThisWnd = FindSubclassWnd(hTrayWnd);
	if (pThisWnd != nullptr)
	{
		pThisWnd->Unsubclass();
		RemoveSubclassItem(hTrayWnd);

		delete pThisWnd;
	}
}

BOOL IsTaskbarHooked(HWND hTaskbar)
{
	if (IsWndSubclassed(hTaskbar))
		return TRUE;

	int rtn = (int)::SendMessage(hTaskbar, MSG_SETTASKBAR_APPEARANCE, (WPARAM)8, (LPARAM)0);
	if (rtn == 42)
		return TRUE;

	return FALSE;
}
