#include "stdafx.h"
#include "UxthemeFunc.h"
#include "ThemeMode.h"

LPCTSTR CUxthemeFunc::m_lpszuxthemeDll = _T("uxtheme.dll");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUxthemeFunc::CUxthemeFunc()
{
	m_bLoad = FALSE;
	m_hModule = NULL;

	m_pfGetCurrentThemeName = NULL;
	m_pfShouldAppsUseDarkMode = NULL;
	m_pfAllowDarkModeForWindow = NULL;
	m_Ordinal135.p = NULL;
	m_pfFlushMenuThemes = NULL;
	m_pfRefreshImmersiveColorPolicyState = NULL;
	m_pfIsDarkModeAllowedForWindow = NULL;
	m_pfGetIsImmersiveColorUsingHighContrast = NULL;
	m_pfOpenNcThemeData = NULL;
	m_pfShouldSystemUseDarkMode = NULL;
	m_pfIsDarkModeAllowedForApp = NULL;
}

CUxthemeFunc::~CUxthemeFunc()
{
	FreeFunctions();
}

BOOL CUxthemeFunc::LoadFunctions()
{
	m_hModule = ::GetModuleHandle(m_lpszuxthemeDll);
	if(m_hModule == NULL)
	{
		m_hModule = ::LoadLibrary(m_lpszuxthemeDll);
		m_bLoad = TRUE;
	}

	if(m_hModule == NULL) //still NULL here? Maybe a 9x OS
	{
		m_bLoad = FALSE;
		return FALSE;
	}
	m_pfGetCurrentThemeName = (FP_GETCURRENTTHEMENAME)GetProcAddress(m_hModule, "GetCurrentThemeName");

	m_pfShouldAppsUseDarkMode = (FP_SHOULDAPPSUSEDARKMODE)::GetProcAddress(m_hModule, MAKEINTRESOURCEA(132));
	m_pfAllowDarkModeForWindow = (FP_ALLOWDARKMODEFORWINDOW)::GetProcAddress(m_hModule, MAKEINTRESOURCEA(133));
	if(GetSystemBuildNumbers() < 18362)
		m_Ordinal135.pfAllowDarkModeForApp = (FP_ALLOWDARKMODEFORAPP)::GetProcAddress(m_hModule, MAKEINTRESOURCEA(135));
	else
		m_Ordinal135.pfSetPreferredAppMode = (FP_SETPREFERREDAPPMODE)::GetProcAddress(m_hModule, MAKEINTRESOURCEA(135));
	m_pfFlushMenuThemes = (FP_FLUSHMENUTHEMES)::GetProcAddress(m_hModule, MAKEINTRESOURCEA(136));
	m_pfRefreshImmersiveColorPolicyState = (FP_REFRESHIMMERSIVECOLORPOLICYSTATE)::GetProcAddress(m_hModule, MAKEINTRESOURCEA(104));
	m_pfIsDarkModeAllowedForWindow = (FP_ISDARKMODEALLOWEDFORWINDOW)::GetProcAddress(m_hModule, MAKEINTRESOURCEA(137));
	m_pfGetIsImmersiveColorUsingHighContrast = (FP_GETISIMMERSIVECOLORUSINGHIGHCONTRAST)::GetProcAddress(m_hModule, MAKEINTRESOURCEA(106));
	m_pfOpenNcThemeData = (FP_OPENNCTHEMEDATA)::GetProcAddress(m_hModule, MAKEINTRESOURCEA(49));
	m_pfShouldSystemUseDarkMode = (FP_SHOULDSYSTEMUSEDARKMODE)::GetProcAddress(m_hModule, MAKEINTRESOURCEA(138));
	m_pfIsDarkModeAllowedForApp = (FP_ISDARKMODEALLOWEDFORAPP)::GetProcAddress(m_hModule, MAKEINTRESOURCEA(139));
	

#ifdef _UNICODE	
#else
#endif
	if((m_pfGetCurrentThemeName == NULL)
		|| (m_pfShouldAppsUseDarkMode == NULL)
		|| (m_pfAllowDarkModeForWindow == NULL)
		|| (m_Ordinal135.p == NULL)
		|| (m_pfFlushMenuThemes == NULL)
		|| (m_pfRefreshImmersiveColorPolicyState == NULL)
		|| (m_pfIsDarkModeAllowedForWindow == NULL)
		|| (m_pfGetIsImmersiveColorUsingHighContrast == NULL)
		|| (m_pfOpenNcThemeData == NULL)
		|| (m_pfShouldSystemUseDarkMode == NULL)
		|| (m_pfIsDarkModeAllowedForApp == NULL))
	{
		FreeFunctions();
		return FALSE;
	}

	return TRUE;
}

BOOL CUxthemeFunc::FreeFunctions()
{
	if(m_hModule != NULL)
	{
		if(m_bLoad)
			::FreeLibrary(m_hModule);

		m_hModule = NULL;
	}

	m_pfGetCurrentThemeName = NULL;
	m_pfShouldAppsUseDarkMode = NULL;
	m_pfAllowDarkModeForWindow = NULL;
	m_Ordinal135.p = NULL;
	m_pfFlushMenuThemes = NULL;
	m_pfRefreshImmersiveColorPolicyState = NULL;
	m_pfIsDarkModeAllowedForWindow = NULL;
	m_pfGetIsImmersiveColorUsingHighContrast = NULL;
	m_pfOpenNcThemeData = NULL;
	m_pfShouldSystemUseDarkMode = NULL;
	m_pfIsDarkModeAllowedForApp = NULL;

	return TRUE;
}

HRESULT CUxthemeFunc::GetCurrentThemeName(LPWSTR pszThemeFileName, int cchMaxNameChars,
	LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars)
{
	if (m_pfGetCurrentThemeName != NULL)
		return m_pfGetCurrentThemeName(pszThemeFileName, cchMaxNameChars, pszColorBuff, cchMaxColorChars, pszSizeBuff, cchMaxSizeChars);
	else
		return S_FALSE;
}

bool CUxthemeFunc::ShouldAppsUseDarkMode()
{
	if(m_pfShouldAppsUseDarkMode != NULL)
		return m_pfShouldAppsUseDarkMode();
	else
		return false;
}

bool CUxthemeFunc::AllowDarkModeForWindow(HWND hWnd, bool allow)
{
	if(m_pfAllowDarkModeForWindow != NULL)
		return m_pfAllowDarkModeForWindow(hWnd, allow);
	else
		return false;
}

bool CUxthemeFunc::AllowDarkModeForApp(bool allow)
{
	if (m_Ordinal135.pfAllowDarkModeForApp != NULL)
		return m_Ordinal135.pfAllowDarkModeForApp(allow);
	else
		return false;
}

PreferredAppMode CUxthemeFunc::SetPreferredAppMode(PreferredAppMode appMode)
{
	if (m_Ordinal135.pfSetPreferredAppMode != NULL)
		return m_Ordinal135.pfSetPreferredAppMode(appMode);
	else
		return Default;
}

void CUxthemeFunc::FlushMenuThemes()
{
	if (m_pfFlushMenuThemes != NULL)
		m_pfFlushMenuThemes();
}

void CUxthemeFunc::RefreshImmersiveColorPolicyState()
{
	if (m_pfRefreshImmersiveColorPolicyState != NULL)
		m_pfRefreshImmersiveColorPolicyState();
}

bool CUxthemeFunc::IsDarkModeAllowedForWindow(HWND hWnd)
{
	if (m_pfIsDarkModeAllowedForWindow != NULL)
		return m_pfIsDarkModeAllowedForWindow(hWnd);
	else
		return false;
}

bool CUxthemeFunc::GetIsImmersiveColorUsingHighContrast(IMMERSIVE_HC_CACHE_MODE mode)
{
	if (m_pfGetIsImmersiveColorUsingHighContrast != NULL)
		return m_pfGetIsImmersiveColorUsingHighContrast(mode);
	else
		return false;
}

HTHEME CUxthemeFunc::OpenNcThemeData(HWND hWnd, LPCWSTR pszClassList)
{
	if (m_pfOpenNcThemeData != NULL)
		return m_pfOpenNcThemeData(hWnd, pszClassList);
	else
		return NULL;
}

bool CUxthemeFunc::ShouldSystemUseDarkMode()
{
	if (m_pfShouldSystemUseDarkMode != NULL)
		return m_pfShouldSystemUseDarkMode();
	else
		return false;
}

bool CUxthemeFunc::IsDarkModeAllowedForApp()
{
	if (m_pfIsDarkModeAllowedForApp != NULL)
		return m_pfIsDarkModeAllowedForApp();
	else
		return false;
}
