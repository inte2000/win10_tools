#include "framework.h"
#include "SubclassMgmt.h"

static std::vector<SubclassItem> s_SubclassMaps;
static CRITICAL_SECTION s_CriticalSection;

static CHookShellTray*FindSubclassHooker(HWND hWnd)
{
	for (auto x : s_SubclassMaps)
	{
		if (x.hWnd == hWnd)
			return x.pHooker;
	}

	return nullptr;
}

void InitSubclassMgmt()
{
	::InitializeCriticalSection(&s_CriticalSection);
}

void ReleaseSubclassMgmt()
{
	::DeleteCriticalSection(&s_CriticalSection);
}

BOOL AddSubclassItem(HWND hWnd, CHookShellTray*pHooker)
{
	::EnterCriticalSection(&s_CriticalSection);

	if (FindSubclassHooker(hWnd) == nullptr)
	{
		s_SubclassMaps.push_back({ hWnd , pHooker});
	}
	::LeaveCriticalSection(&s_CriticalSection);

	return TRUE;
}

void RemoveSubclassItem(HWND hWnd)
{
	::EnterCriticalSection(&s_CriticalSection);

	for (auto it = s_SubclassMaps.begin(); it != s_SubclassMaps.end(); ++it)
	{
		if (it->hWnd == hWnd)
		{
			s_SubclassMaps.erase(it);
			break;
		}
	}

	::LeaveCriticalSection(&s_CriticalSection);
}

BOOL IsWndSubclassed(HWND hWnd)
{
	CHookShellTray*pHooker = nullptr;

	::EnterCriticalSection(&s_CriticalSection);
	pHooker = FindSubclassHooker(hWnd);
	::LeaveCriticalSection(&s_CriticalSection);

	return (pHooker != nullptr);
}

CHookShellTray*FindSubclassWnd(HWND hWnd)
{
	CHookShellTray*pHooker = nullptr;

	::EnterCriticalSection(&s_CriticalSection);
	pHooker = FindSubclassHooker(hWnd);
	::LeaveCriticalSection(&s_CriticalSection);

	return pHooker;
}
